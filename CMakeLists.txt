# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mcx16") 

find_package(TBB REQUIRED)
find_package(LibCDS REQUIRED)
find_package(Threads REQUIRED)

project(test)
include_directories(".")
add_subdirectory(matplotplusplus)

add_executable(hashmap hashmap.cpp)
add_executable(queue queue.cpp)

add_executable(plot plot.cpp)

target_link_libraries(plot PUBLIC matplot)
target_link_libraries(hashmap TBB::tbb LibCDS::cds Threads::Threads)
target_link_libraries(queue TBB::tbb LibCDS::cds Threads::Threads)
