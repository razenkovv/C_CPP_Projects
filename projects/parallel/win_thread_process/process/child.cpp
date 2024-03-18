#include <Windows.h>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "methods.hpp"

int main(int argc, char const* argv[]) {
  unsigned int number = strtoul(argv[0], NULL, 10);
  unsigned int raw_data_offset = strtoul(argv[1], NULL, 10);
  unsigned int image_offset = strtoul(argv[2], NULL, 10);
  unsigned int result_offset = strtoul(argv[3], NULL, 10);
  unsigned int args_offset = strtoul(argv[4], NULL, 10);
  unsigned int n_processes = strtoul(argv[5], NULL, 10);
  unsigned int size_of_shared_memory = strtoul(argv[6], NULL, 10);

  char MAP_NAME[] = "MAP_NAME";
  HANDLE mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, MAP_NAME);
  if (mapping == NULL) {
    std::cerr << "Could not open file mapping object: " << GetLastError() << std::endl;
    exit(1);
  }

  unsigned char* shared_memory_ptr = (unsigned char*)MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, size_of_shared_memory);
  if (shared_memory_ptr == NULL) {
    std::cout << "Could not map view of file: " << GetLastError() << std::endl;
    CloseHandle(mapping);
    exit(1);
  }

  std::vector<unsigned int> begins;
  std::vector<unsigned int> ends;
  for (int j = 0; j < n_processes; ++j) {
    begins.push_back(get_4b(shared_memory_ptr, args_offset + (number * n_processes * 2 + j) * sizeof(unsigned int)));
    ends.push_back(get_4b(shared_memory_ptr, args_offset + (number * n_processes * 2 + j + n_processes) * sizeof(unsigned int)));
  }

  BMP* image = (BMP*)(shared_memory_ptr + image_offset);
  // image->raw_data_ = shared_memory_ptr + raw_data_offset;

  ImageColors local_res{0, 0, 0};
  for (int j = 0; j < n_processes; ++j) {
    // local_res += part_clr_prev(image, begins[j], ends[j]);
    local_res += part_clr_prev_2(shared_memory_ptr, begins[j], ends[j], image->height_, image->width_, image->offset_, image->padding_, image->channels_);
  }

  set_4b(shared_memory_ptr, result_offset + 3 * number * sizeof(unsigned int), local_res.r_amount_);
  set_4b(shared_memory_ptr, result_offset + 3 * number * sizeof(unsigned int) + sizeof(unsigned int), local_res.g_amount_);
  set_4b(shared_memory_ptr, result_offset + 3 * number * sizeof(unsigned int) + 2 * sizeof(unsigned int), local_res.b_amount_);

  UnmapViewOfFile(shared_memory_ptr);
  CloseHandle(mapping);

  return 0;
}