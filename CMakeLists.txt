# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcx16") 

find_package(TBB REQUIRED)
find_package(LibCDS REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem thread)

project(test)
include_directories(".")
add_subdirectory(matplotplusplus)

add_executable(hashmap hashmap.cpp)
add_executable(queue_main queue_main.cpp)

add_executable(plot plot.cpp)

target_link_libraries(plot PUBLIC matplot)
target_link_libraries(hashmap TBB::tbb LibCDS::cds Threads::Threads)
target_link_libraries(queue_main TBB::tbb LibCDS::cds Threads::Threads Boost::thread)
