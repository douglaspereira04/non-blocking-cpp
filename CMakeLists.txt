# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp -ltbb") 
project(test)
include_directories(".")

add_executable(main main.cpp)

#find_package(LibCDS CONFIG REQUIRED)
#target_link_libraries(main PRIVATE LibCDS::cds-s)

