#pragma once

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
ImageColors win_thread_color_prevalence(BMP& image, unsigned int n_threads);
ImageColors part_clr_prev(BMP& image, unsigned int begin_column, unsigned int end_column);

// structure of arguments for passing to CreateThread
struct Args {
public:
  BMP& image_;
  std::vector<unsigned int> begins_;  // [ begins_[i], ends_[i] ) - work for i-th process
  std::vector<unsigned int> ends_;
  unsigned int number_;
  std::vector<ImageColors>& res_;
  
  Args(BMP& image, unsigned int n_threads, std::vector<ImageColors>& res) : image_(image), begins_(n_threads), ends_(n_threads), number_(0), res_(res) {}
};