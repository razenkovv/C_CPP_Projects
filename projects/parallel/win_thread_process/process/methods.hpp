#pragma once

#include <vector>

#include "bmp/bmp.h"

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
ImageColors win_process_color_prevalence(BMP& image, unsigned int n_processes);
ImageColors part_clr_prev(BMP* image, unsigned int begin_column, unsigned int end_column);
ImageColors part_clr_prev_2(unsigned char* raw_data,
                            unsigned int begin_column,
                            unsigned int end_column,
                            unsigned int height,
                            unsigned int width,
                            unsigned int offset,
                            unsigned int padding,
                            unsigned int channels);
unsigned int get_4b(unsigned char* buffer, unsigned int i);
void set_4b(unsigned char* buffer, unsigned int i, unsigned int v);

struct ProcessArg {
 public:
  std::vector<unsigned int> begins_;
  std::vector<unsigned int> ends_;

  ProcessArg(unsigned int n) : begins_(n), ends_(n) {}
};