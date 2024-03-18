#include <mpi.h>
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

ImageColors part_clr_prev(BMP* image, unsigned int begin_column, unsigned int end_column) {
  ImageColors res{0, 0, 0};
  for (unsigned int j = 0; j < image->height_; ++j) {
    for (unsigned int i = begin_column; i < end_column; ++i) {
      RGB rgb = get_pixel_value(image, i, j);
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

BMP create_image(unsigned char* buf, unsigned int size, unsigned int offset, unsigned int width, unsigned int height, unsigned int row_size) {
  BMP image;
  image.raw_data_ = buf;
  image.size_ = size;
  image.offset_ = offset;
  image.width_ = width;
  image.height_ = height;
  image.bits_per_pixel_ = 24;
  image.channels_ = 3;
  image.compression_ = 0;
  image.row_size_ = row_size;
  image.padding_ = image.row_size_ - image.width_ * image.channels_;
  return image;
}