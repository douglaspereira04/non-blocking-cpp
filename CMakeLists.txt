# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") 

add_subdirectory(WaitFreeCollections)
find_package(Threads REQUIRED)
find_package(TBB REQUIRED)
project(test)
include_directories(".")

add_executable(main main.cpp)
target_link_libraries(main Threads::Threads WaitFreeCollections TBB::tbb)

#find_package(LibCDS CONFIG REQUIRED)
#target_link_libraries(main PRIVATE LibCDS::cds-s)

