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
ImageColors part_clr_prev(BMP* image, unsigned int begin_column, unsigned int end_column);

// for 24-bits images
BMP create_image(unsigned char* buf, unsigned int size, unsigned int offset, unsigned int width, unsigned int height, unsigned int row_size);

struct ProcessArg {
 public:
  std::vector<unsigned int> begins_;
  std::vector<unsigned int> ends_;

  ProcessArg(unsigned int n) : begins_(n), ends_(n) {}
};

struct ProcessInfo {
 public:
  unsigned int height_;
  unsigned int width_;
  unsigned int row_size_;
  unsigned int count_;
  unsigned int offset_;

  ProcessInfo() : height_(0), width_(0), row_size_(0), count_(0), offset_(0) {}
  ProcessInfo(unsigned int height, unsigned int width, unsigned int row_size, unsigned int count, unsigned int offset)
      : height_(height), width_(width), row_size_(row_size), count_(count), offset_(offset) {}
};