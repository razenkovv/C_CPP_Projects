#pragma once

#include <thread>
#include <vector>

#include "bmp.h"

struct ImageColors {
  unsigned int r_amount_;
  unsigned int g_amount_;
  unsigned int b_amount_;

  ImageColors& operator+=(const ImageColors& other) {
    r_amount_ += other.r_amount_;
    g_amount_ += other.g_amount_;
    b_amount_ += other.b_amount_;
    return *this;
  }

  bool operator==(const ImageColors& other) const { return r_amount_ == other.r_amount_ && g_amount_ == other.g_amount_ && b_amount_ == other.b_amount_; }
};

void to_grayscale(BMP& image);
ImageColors brute_color_prevalence(BMP& image);
ImageColors posix_thread_color_prevalence(BMP& image, unsigned int n_threads);
ImageColors std_thread_color_prevalence(BMP& image, unsigned int n_threads);
ImageColors omp_thread_color_prevalence(BMP& image, unsigned int n_threads);
ImageColors part_clr_prev(BMP& image, unsigned int begin_column, unsigned int end_column);

// structure of arguments for passing to pthread_create
struct Args {
 public:
  BMP& image_;
  std::vector<unsigned int> begins_;  // [ begins_[i], ends_[i] ) - work for i-th process
  std::vector<unsigned int> ends_;
  unsigned int number_;
  std::vector<ImageColors>& res_;

  Args(BMP& image, unsigned int n_threads, std::vector<ImageColors>& res) : image_(image), begins_(n_threads), ends_(n_threads), number_(0), res_(res) {}
};

struct Partition {
  std::vector<unsigned int> begins_;
  std::vector<unsigned int> ends_;
  Partition(unsigned int n) : begins_(n), ends_(n) {}
};

class SolverParallel {
 private:
  BMP& image_;
  std::vector<std::thread> threads_;
  std::vector<Partition> parts_;
  std::vector<unsigned int> shifts_;
  unsigned int n_threads_;

  std::mutex lock_;
  void std_color_prevalence(unsigned int number);

 public:
  ImageColors aggregate_result_ = {0, 0, 0};
  SolverParallel(BMP& image, unsigned int n_threads);
  ImageColors std_thread_color_prevalence();
};