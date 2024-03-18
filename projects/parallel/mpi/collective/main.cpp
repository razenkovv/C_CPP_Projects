#include <mpi.h>
#include <chrono>
#include <cstddef>
#include <cstring>
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
  unsigned char* buffer;
  unsigned int* shifts;
  ImageColors* results;
  ProcessInfo pi;
  auto begin = std::chrono::steady_clock::now();

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
  int blocklengths1[5] = {1, 1, 1, 1, 1};
  MPI_Aint displacements1[5];
  MPI_Datatype oldtypes1[5] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
  displacements1[0] = offsetof(ProcessInfo, height_);
  displacements1[1] = offsetof(ProcessInfo, width_);
  displacements1[2] = offsetof(ProcessInfo, row_size_);
  displacements1[3] = offsetof(ProcessInfo, count_);
  displacements1[4] = offsetof(ProcessInfo, offset_);

  MPI_Type_create_struct(5, blocklengths1, displacements1, oldtypes1, &MPI_PROCESS_INFO);
  MPI_Type_commit(&MPI_PROCESS_INFO);

  if (world_rank == 0) {
    begin = std::chrono::steady_clock::now();
    BMP image = bmp_read("bmp/test.bmp");
    buffer = new unsigned char[image.size_];
    std::memcpy(buffer, image.raw_data_, image.size_);

    shifts = new unsigned int[n_processes + 1];
    for (int i = 0; i <= n_processes; ++i) {
      shifts[i] = i * image.width_ / n_processes;
    }

    pi = ProcessInfo{image.height_, image.width_, image.row_size_, image.size_, image.offset_};
    results = new ImageColors[n_processes];
    bmp_close(&image);
  }

  MPI_Bcast(&pi, 1, MPI_PROCESS_INFO, 0, MPI_COMM_WORLD);

  if (world_rank != 0) {
    buffer = new unsigned char[pi.count_];
    shifts = new unsigned int[n_processes + 1];
  }

  MPI_Bcast(shifts, n_processes + 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  MPI_Bcast(buffer, pi.count_, MPI_BYTE, 0, MPI_COMM_WORLD);

  BMP p_image = create_image(buffer, pi.count_, pi.offset_, pi.width_, pi.height_, pi.row_size_);
  std::vector<unsigned int> begins(n_processes);
  std::vector<unsigned int> ends(n_processes);

  for (int i = 0; i < n_processes; ++i) {
    begins[i] = shifts[i] + world_rank * (shifts[i + 1] - shifts[i]) / n_processes;
    ends[i] = shifts[i] + (world_rank + 1) * (shifts[i + 1] - shifts[i]) / n_processes;
  }

  ImageColors res{};
  for (int i = 0; i < n_processes; ++i) {
    res += part_clr_prev(&p_image, begins[i], ends[i]);
  }
  MPI_Gather(&res, 1, MPI_IMAGE_COLORS, results, 1, MPI_IMAGE_COLORS, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  if (world_rank == 0) {
    ImageColors res{};
    for (int i = 0; i < n_processes; ++i) {
      res += results[i];
    }
    delete[] results;
    auto end = std::chrono::steady_clock::now();

    std::cout << "Red: " << res.r_amount_ << " Green: " << res.g_amount_ << " Blue: " << res.b_amount_ << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1e-6 << " seconds" << std::endl;

    auto begin2 = std::chrono::steady_clock::now();
    auto res_brute = brute_color_prevalence(p_image);
    auto end2 = std::chrono::steady_clock::now();

    std::cout << "\nRed_brute: " << res_brute.r_amount_ << " Green_brute: " << res_brute.g_amount_ << " Blue_brute: " << res_brute.b_amount_ << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() * 1e-6 << " seconds" << std::endl;

    if (res_brute == res)
      std::cout << "\nCorrect: YES" << std::endl;
    else
      std::cout << "\nCorrect: NO" << std::endl;
  }

  MPI_Type_free(&MPI_IMAGE_COLORS);
  MPI_Type_free(&MPI_PROCESS_INFO);
  delete[] buffer;
  delete[] shifts;
  MPI_Finalize();
  return 0;
}