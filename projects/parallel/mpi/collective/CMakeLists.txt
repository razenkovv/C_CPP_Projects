cmake_minimum_required(VERSION 3.20)
project(parallel_programming)

find_package(MPI REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_executable(run main.cpp methods.cpp)
add_subdirectory(bmp/)

target_link_libraries(run PRIVATE MPI::MPI_CXX bmp)

target_include_directories(run PRIVATE bmp/)