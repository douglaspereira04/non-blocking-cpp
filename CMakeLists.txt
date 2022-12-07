# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -mcx16") 

find_package(TBB REQUIRED)
find_package(LibCDS REQUIRED)

project(test)
include_directories(".")
add_executable(main main.cpp)

target_link_libraries(main TBB::tbb LibCDS::cds)

