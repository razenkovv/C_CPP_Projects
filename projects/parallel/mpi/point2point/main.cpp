#include <mpi.h>
#include <chrono>
#include <cstddef>
#include <iostream>

#include "bmp.h"
#include "methods.hpp"

// X = 3 * 205 + 12 = 627
// A = X % 4 = 3
// B = 7 + X % 7 = 11

// ./run.sh 7

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int n_processes = std::stoul(argv[1]);
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // creating new types
  MPI_Datatype MPI_IMAGE_COLORS;
  int blocklengths[3] = {1, 1, 1};
  MPI_Aint displacements[3];
  MPI_Datatype oldtypes[3] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
  displacements[0] = offsetof(ImageColors, r_amount_);
  displacements[1] = offsetof(ImageColors, g_amount_);
  displacements[2] = offsetof(ImageColors, b_amount_);
  MPI_Type_create_struct(3, blocklengths, displacements, oldtypes, &MPI_IMAGE_COLORS);
  MPI_Type_commit(&MPI_IMAGE_COLORS);

  MPI_Datatype MPI_PROCESS_INFO;
  int blocklengths1[4] = {1, 1, 1, 1};
  MPI_Aint displacements1[4];
  MPI_Datatype oldtypes1[4] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
  displacements1[0] = offsetof(ProcessInfo, height_);
  displacements1[1] = offsetof(ProcessInfo, width_);
  displacements1[2] = offsetof(ProcessInfo, row_size_);
  displacements1[3] = offsetof(ProcessInfo, count_);
  MPI_Type_create_struct(4, blocklengths1, displacements1, oldtypes1, &MPI_PROCESS_INFO);
  MPI_Type_commit(&MPI_PROCESS_INFO);

  if (world_rank == 0) {
    auto begin = std::chrono::steady_clock::now();
    BMP image = bmp_read("bmp/test.bmp");
    std::vector<unsigned int> shifts(n_processes + 1);
    for (int i = 0; i <= n_processes; ++i) {
      shifts[i] = i * image.height_ / n_processes;
    }
    std::vector<ProcessArg> args(n_processes, ProcessArg(n_processes));
    for (int i = 0; i < n_processes; ++i) {
      for (int j = 0; j < n_processes; ++j) {
        args[j].begins_[i] = shifts[i] + j * (shifts[i + 1] - shifts[i]) / n_processes;
        args[j].ends_[i] = shifts[i] + (j + 1) * (shifts[i + 1] - shifts[i]) / n_processes;
      }
    }

    for (int i = 1; i < n_processes; ++i) {
      for (int j = 0; j < n_processes; ++j) {
        unsigned int offset = get_pixel(&image, 0, args[i].begins_[j]);
        ProcessInfo pi{args[i].ends_[j] - args[i].begins_[j], image.width_, image.row_size_, (args[i].ends_[j] - args[i].begins_[j]) * image.row_size_};
        MPI_Send(&pi, 1, MPI_PROCESS_INFO, i, 0, MPI_COMM_WORLD);
        MPI_Send(image.raw_data_ + offset, pi.count_, MPI_BYTE, i, 0, MPI_COMM_WORLD);
      }
    }

    ImageColors res{};
    for (int j = 0; j < n_processes; ++j) {
      int offset = get_pixel(&image, 0, args[0].begins_[j]);
      int n = args[0].ends_[j] - args[0].begins_[j];
      int count = n * image.row_size_;
      ImageColors local_res{};
      BMP part_image = create_image(image.raw_data_ + offset, count, 0, image.width_, n, image.row_size_);
      local_res = brute_color_prevalence(part_image);
      res += local_res;
    }

    for (int i = 1; i < n_processes; ++i) {
      ImageColors res_;
      MPI_Recv(&res_, 1, MPI_IMAGE_COLORS, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      res += res_;
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Red: " << res.r_amount_ << " Green: " << res.g_amount_ << " Blue: " << res.b_amount_ << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1e-6 << " seconds" << std::endl;

    auto begin2 = std::chrono::steady_clock::now();
    auto res_brute = brute_color_prevalence(image);
    auto end2 = std::chrono::steady_clock::now();

    std::cout << "\nRed_brute: " << res_brute.r_amount_ << " Green_brute: " << res_brute.g_amount_ << " Blue_brute: " << res_brute.b_amount_ << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() * 1e-6 << " seconds" << std::endl;

    if (res_brute == res)
      std::cout << "\nCorrect: YES" << std::endl;
    else
      std::cout << "\nCorrect: NO" << std::endl;

    bmp_close(&image);
  }

  if (world_rank != 0) {
    ImageColors res{};
    for (int i = 0; i < n_processes; ++i) {
      ProcessInfo pi;
      MPI_Recv(&pi, 1, MPI_PROCESS_INFO, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      unsigned char* buffer = new unsigned char[pi.count_];
      MPI_Recv(buffer, pi.count_, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      BMP part_image = create_image(buffer, pi.count_, 0, pi.width_, pi.height_, pi.row_size_);
      ImageColors local_res = brute_color_prevalence(part_image);
      res += local_res;
      delete[] buffer;
    }
    MPI_Send(&res, 1, MPI_IMAGE_COLORS, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Type_free(&MPI_IMAGE_COLORS);
  MPI_Type_free(&MPI_PROCESS_INFO);
  MPI_Finalize();
  return 0;
}