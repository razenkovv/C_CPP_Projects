#include <windows.h>
#include <bitset>
#include <cstring>
#include <iostream>
#include <vector>

#include "methods.hpp"

unsigned int get_4b(unsigned char* buffer, unsigned int i) {
  return static_cast<unsigned int>(buffer[i]) | (static_cast<unsigned int>(buffer[i + 1]) << 8) | (static_cast<unsigned int>(buffer[i + 2]) << 16) |
         (static_cast<unsigned int>(buffer[i + 3]) << 24);
}

void set_4b(unsigned char* buffer, unsigned int i, unsigned int v) {
  *((unsigned int*)(buffer + i)) = v;
}

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

ImageColors part_clr_prev_2(unsigned char* raw_data,
                            unsigned int begin_column,
                            unsigned int end_column,
                            unsigned int height,
                            unsigned int width,
                            unsigned int offset,
                            unsigned int padding,
                            unsigned int channels) {
  ImageColors res{0, 0, 0};
  for (unsigned int j = 0; j < height; ++j) {
    for (unsigned int i = begin_column; i < end_column; ++i) {
      unsigned int pixel = offset + j * (width * channels + padding) + i * channels;
      RGB rgb = {.red_ = raw_data[pixel + 2], .green_ = raw_data[pixel + 1], .blue_ = raw_data[pixel]};
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

//---------------------------------------------------------------------------------------------------------------------------------------

ImageColors win_process_color_prevalence(BMP& image, unsigned int n_processes) {
  char MAP_NAME[] = "MAP_NAME";
  unsigned int size_of_shared_memory = image.size_ + sizeof(image) + n_processes * 3 * sizeof(unsigned int) + n_processes * n_processes * 2 * sizeof(unsigned int);
  HANDLE mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size_of_shared_memory, MAP_NAME);
  if (mapping == NULL) {
    std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
    exit(1);
  }

  unsigned char* shared_memory_ptr = (unsigned char*)MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, size_of_shared_memory);
  if (shared_memory_ptr == NULL) {
    std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
    CloseHandle(mapping);
    exit(1);
  }

  unsigned int raw_data_offset = 0;
  unsigned int image_offset = image.size_;
  unsigned int result_offset = image_offset + sizeof(image);
  unsigned int args_offset = result_offset + n_processes * 3 * sizeof(unsigned int);

  std::memset(shared_memory_ptr, '\0', size_of_shared_memory);                     
  std::memcpy(shared_memory_ptr + raw_data_offset, image.raw_data_, image.size_);  
  std::memcpy(shared_memory_ptr + image_offset, &image, sizeof(image));       

  std::vector<unsigned int> shifts(n_processes + 1);
  for (int i = 0; i <= n_processes; ++i) {
    shifts[i] = i * image.width_ / n_processes;
  }

  std::vector<ProcessArg> args(n_processes, ProcessArg(n_processes));

  for (int i = 0; i < n_processes; ++i) {
    for (int j = 0; j < n_processes; ++j) {
      args[j].begins_[i] = shifts[i] + j * (shifts[i + 1] - shifts[i]) / n_processes;
      args[j].ends_[i] = shifts[i] + (j + 1) * (shifts[i + 1] - shifts[i]) / n_processes;
    }
  }

  unsigned int counter = args_offset;
  for (auto& arg : args) {
    std::memcpy(shared_memory_ptr + counter, &arg.begins_[0], n_processes * sizeof(unsigned int));
    counter += n_processes * sizeof(unsigned int);
    std::memcpy(shared_memory_ptr + counter, &arg.ends_[0], n_processes * sizeof(unsigned int));
    counter += n_processes * sizeof(unsigned int);
  }

  std::vector<STARTUPINFOA> si{n_processes};
  std::vector<PROCESS_INFORMATION> pi{n_processes};
  char child[] = "child.exe";

  for (int i = 0; i < n_processes; ++i) {
    char cmd_args[256];
    std::memset(cmd_args, '\0', 256);
    sprintf(cmd_args, "%d %ld %ld %ld %ld %ld %ld", i, raw_data_offset, image_offset, result_offset, args_offset, n_processes, size_of_shared_memory);
    ZeroMemory(&si[i], sizeof(si[i]));
    si[i].cb = sizeof(si[i]);
    ZeroMemory(&pi[i], sizeof(pi[i]));
    bool bProcess = CreateProcessA(child, cmd_args, NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i]);
    if (bProcess == false) {
      std::cout << "CreateProcess failed: " << GetLastError() << std::endl;
    }
  }

  for (int i = 0; i < n_processes; ++i) {
    WaitForSingleObject(pi[i].hProcess, INFINITE);
  }

  for (int i = 0; i < n_processes; ++i) {
    CloseHandle(pi[i].hProcess);
    CloseHandle(pi[i].hThread);
  }

  ImageColors res{0, 0, 0};
  for (int i = 0; i < n_processes; ++i) {
    res.r_amount_ += get_4b(shared_memory_ptr, result_offset + 3 * i * sizeof(unsigned int));
    res.g_amount_ += get_4b(shared_memory_ptr, result_offset + 3 * i * sizeof(unsigned int) + sizeof(unsigned int));
    res.b_amount_ += get_4b(shared_memory_ptr, result_offset + 3 * i * sizeof(unsigned int) + 2 * sizeof(unsigned int));
  }

  UnmapViewOfFile(shared_memory_ptr);
  CloseHandle(mapping);

  return res;
}