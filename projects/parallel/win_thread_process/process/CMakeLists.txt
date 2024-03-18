cmake_minimum_required(VERSION 3.20)
project(parallel_programming)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_executable(run main.cpp methods.cpp)
add_executable(child child.cpp methods.cpp)

add_subdirectory(bmp/)

target_link_libraries(run bmp)
target_link_libraries(child bmp)

target_include_directories(run PRIVATE bmp/)
target_include_directories(child PRIVATE bmp/)

# target_compile_options(run PRIVATE /fsanitize=address)
# target_compile_options(child PRIVATE /fsanitize=address)