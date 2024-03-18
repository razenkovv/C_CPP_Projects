cmake_minimum_required(VERSION 3.20)
project(t_flex LANGUAGES CXX)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(run main.cpp visual.cpp)
 
target_link_libraries (run Eigen3::Eigen)
target_compile_features(run PRIVATE cxx_std_20)
target_link_libraries(run sfml-graphics)