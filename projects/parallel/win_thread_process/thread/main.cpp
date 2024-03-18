#include <windows.h>

#include <chrono>
#include <iostream>

#include "bmp.h"
#include "methods.hpp"

// X = 3 * 205 + 12 = 627
// A = X % 4 = 3
// B = 7 + X % 7 = 11

int main() {
  unsigned int n{};
  std::cout << "n_threads: ";
  std::cin >> n;
  std::cout << std::endl;

  BMP image = bmp_read("..\\bmp\\test.bmp");
  auto begin = std::chrono::steady_clock::now();
  auto res = win_thread_color_prevalence(image, n);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Red: " << res.r_amount_ << " Green: " << res.g_amount_ << " Blue: " << res.b_amount_ << std::endl;
  std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() * 1e-6 << " seconds" << std::endl;

  auto begin2 = std::chrono::steady_clock::now();
  auto res_brute = brute_color_prevalence(image);
  auto end2 = std::chrono::steady_clock::now();

  std::cout << "\nRed_brute: " << res_brute.r_amount_ << " Green_brute: " << res_brute.g_amount_ << " Blue_brute: " << res_brute.b_amount_ << std::endl;
  std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() * 1e-6 << " seconds" << std::endl;

  if (res_brute == res)
    std::cout << "\nCorrect: YES" << std::endl;
  else
    std::cout << "\nCorrect: NO" << std::endl;

  // to_grayscale(image);
  bmp_write(&image, "..\\bmp\\test_write.bmp");
  bmp_close(&image);

  return 0;
}