#include <windows.h>
#include <iostream>
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

DWORD __stdcall color_prevalence(void* arguments) {
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

ImageColors win_thread_color_prevalence(BMP& image, unsigned int n_threads) {
  std::vector<HANDLE> thread_handles(n_threads);
  std::vector<DWORD> thread_ids(n_threads);

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
    thread_handles[i] = CreateThread(NULL, 0, color_prevalence, (void*)&vector_of_args[i], 0, &thread_ids[i]);
    if (thread_handles[i] == NULL) {
      std::cerr << "Error creating thread: " << i << std::endl;
      return {};
    }
  }

  WaitForMultipleObjects(n_threads, &thread_handles[0], true, INFINITE);

  ImageColors result{0, 0, 0};
  for (int i = 0; i < n_threads; ++i) {
    result += results[i];
  }

  return result;
}