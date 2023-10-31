# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcx16 -O3") 

find_package(TBB REQUIRED)
find_package(LibCDS REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem thread)

project(test)
include_directories(".")
add_subdirectory(matplotplusplus)

add_executable(hashmap hashmap_main.cpp)
add_executable(queue queue_main.cpp)

add_executable(hashmap_plot hashmap_plot.cpp)
add_executable(queue_plot queue_plot.cpp)


option(BIG_VALUES "Compile test with 1024 bytes values" OFF)

IF(BIG_VALUES)
    target_compile_definitions(hashmap PUBLIC BIG)
    target_compile_definitions(queue PUBLIC BIG)
ENDIF(BIG_VALUES)


target_link_libraries(hashmap_plot PUBLIC matplot)
target_link_libraries(queue_plot PUBLIC matplot)
target_link_libraries(hashmap TBB::tbb LibCDS::cds Threads::Threads)
target_link_libraries(queue TBB::tbb LibCDS::cds Threads::Threads Boost::thread)
