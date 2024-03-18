#include <omp.h>
#include <pthread.h>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#include "methods.hpp"

void to_grayscale(BMP& image) {
  for (unsigned int j = 0; j < image.height_; ++j) {
    for (unsigned int i = 0; i < image.width_; ++i) {
      RGB rgb = get_pixel_value(&image, i, j);
      unsigned char grayscale = (unsigned char)(rgb.red_ * 0.2 + rgb.green_ * 0.6 + rgb.blue_ * 0.1) % 256;
      RGB rgb_res = {.red_ = grayscale, .green_ = grayscale, .blue_ = grayscale};
      set_pixel_value(&image, i, j, rgb_res);
    }
  }
}

ImageColors brute_color_prevalence(BMP& image) {
  ImageColors res{0, 0, 0};
  for (unsigned int j = 0; j < image.height_; ++j) {
    for (unsigned int i = 0; i < image.width_; ++i) {
      RGB rgb = get_pixel_value(&image, i, j);
      if (rgb.red_ >= rgb.green_ && rgb.red_ >= rgb.blue_)
        ++res.r_amount_;
      else if (rgb.green_ > rgb.red_ && rgb.green_ >= rgb.blue_)
        ++res.g_amount_;
      else if (rgb.blue_ > rgb.red_ && rgb.blue_ > rgb.green_)
        ++res.b_amount_;
    }
  }
  return res;
}

ImageColors part_clr_prev(BMP& image, unsigned int begin_column, unsigned int end_column) {
  ImageColors res{0, 0, 0};
  for (unsigned int j = 0; j < image.height_; ++j) {
    for (unsigned int i = begin_column; i < end_column; ++i) {
      RGB rgb = get_pixel_value(&image, i, j);
      if (rgb.red_ >= rgb.green_ && rgb.red_ >= rgb.blue_)
        ++res.r_amount_;
      else if (rgb.green_ > rgb.red_ && rgb.green_ >= rgb.blue_)
        ++res.g_amount_;
      else if (rgb.blue_ > rgb.red_ && rgb.blue_ > rgb.green_)
        ++res.b_amount_;
    }
  }
  return res;
}

//------------------------------------------------------------------------------------------------
// posix thread

void* posix_color_prevalence(void* arguments) {
  Args* args = (Args*)arguments;
  unsigned int n_blocks = args->begins_.size();
  ImageColors thread_res{0, 0, 0};
  for (int i = 0; i < n_blocks; ++i) {
    ImageColors res = part_clr_prev(args->image_, args->begins_[i], args->ends_[i]);
    thread_res += res;
  }
  args->res_[args->number_] += thread_res;
  return 0;
}

ImageColors posix_thread_color_prevalence(BMP& image, unsigned int n_threads) {
  std::vector<pthread_t> threads(n_threads);

  std::vector<ImageColors> results(n_threads, ImageColors{0, 0, 0});
  std::vector<Args> vector_of_args(n_threads, Args(image, n_threads, results));
  std::vector<unsigned int> shifts(n_threads + 1);

  for (int i = 0; i <= n_threads; ++i) {
    shifts[i] = i * image.width_ / n_threads;
  }

  for (int i = 0; i < n_threads; ++i) {
    vector_of_args[i].number_ = i;
    for (int j = 0; j < n_threads; ++j) {
      vector_of_args[j].begins_[i] = shifts[i] + j * (shifts[i + 1] - shifts[i]) / n_threads;
      vector_of_args[j].ends_[i] = shifts[i] + (j + 1) * (shifts[i + 1] - shifts[i]) / n_threads;
    }
  }

  std::vector<ImageColors> res(n_threads, ImageColors{0, 0, 0});
  for (int i = 0; i < n_threads; ++i) {
    int check = pthread_create(&threads[i], NULL, posix_color_prevalence, (void*)&vector_of_args[i]);
    if (check != 0) {
      std::cerr << "Error creating thread: " << i << std::endl;
      return {};
    }
  }

  for (int i = 0; i < n_threads; ++i) {
    pthread_join(threads[i], NULL);
  }

  ImageColors result{0, 0, 0};
  for (int i = 0; i < n_threads; ++i) {
    result += results[i];
  }

  return result;
}

//------------------------------------------------------------------------------------------------
// std::thread

SolverParallel::SolverParallel(BMP& image, unsigned int n_threads)
    : image_(image), threads_(), parts_(n_threads, Partition(n_threads)), shifts_(n_threads + 1), n_threads_(n_threads) {
  for (int i = 0; i <= n_threads_; ++i)
    shifts_[i] = i * image_.width_ / n_threads_;
  for (int i = 0; i < n_threads_; ++i) {
    for (int j = 0; j < n_threads_; ++j) {
      parts_[j].begins_[i] = shifts_[i] + j * (shifts_[i + 1] - shifts_[i]) / n_threads_;
      parts_[j].ends_[i] = shifts_[i] + (j + 1) * (shifts_[i + 1] - shifts_[i]) / n_threads_;
    }
  }
}

void SolverParallel::std_color_prevalence(unsigned int number) {
  ImageColors thread_res{0, 0, 0};
  // n_threads == n_blocks
  for (int i = 0; i < SolverParallel::n_threads_; ++i) {
    ImageColors res = part_clr_prev(image_, parts_[number].begins_[i], parts_[number].ends_[i]);
    thread_res += res;
  }

  lock_.lock();
  aggregate_result_ += thread_res;
  lock_.unlock();
}

ImageColors SolverParallel::std_thread_color_prevalence() {
  for (int i = 0; i < n_threads_; ++i) {
    threads_.emplace_back(&SolverParallel::std_color_prevalence, this, i);
  }
  for (auto& t : threads_)
    t.join();

  return aggregate_result_;
}

//------------------------------------------------------------------------------------------------
// OpenMP

void* omp_color_prevalence(void* arguments) {
  Args* args = (Args*)arguments;
  unsigned int n_blocks = args->begins_.size();
  ImageColors thread_res{0, 0, 0};
  for (int i = 0; i < n_blocks; ++i) {
    ImageColors res = part_clr_prev(args->image_, args->begins_[i], args->ends_[i]);
    thread_res += res;
  }
  args->res_[args->number_] += thread_res;
  return 0;
}

ImageColors omp_thread_color_prevalence(BMP& image, unsigned int n_threads) {
  std::vector<ImageColors> results(n_threads, ImageColors{0, 0, 0});
  std::vector<Partition> parts(n_threads, n_threads);
  std::vector<unsigned int> shifts(n_threads + 1);
  ImageColors total_res{0, 0, 0};

  for (int i = 0; i <= n_threads; ++i) {
    shifts[i] = i * image.width_ / n_threads;
  }

  for (int i = 0; i < n_threads; ++i) {
    for (int j = 0; j < n_threads; ++j) {
      parts[j].begins_[i] = shifts[i] + j * (shifts[i + 1] - shifts[i]) / n_threads;
      parts[j].ends_[i] = shifts[i] + (j + 1) * (shifts[i + 1] - shifts[i]) / n_threads;
    }
  }

omp_set_num_threads(n_threads);
#pragma omp parallel for
  for (auto& p : parts) {
    ImageColors local_res{0, 0, 0};
    for (int i = 0; i < n_threads; ++i) {
      auto res = part_clr_prev(image, p.begins_[i], p.ends_[i]);
      local_res += res;
    }

#pragma omp critical
    total_res += local_res;
  }
  return total_res;
}