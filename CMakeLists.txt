# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O1") 

add_library(cds SHARED IMPORTED)
set_target_properties(cds 
    PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/cds/bin/libcds.so.2.3.3)
target_include_directories(cds INTERFACE ${CMAKE_SOURCE_DIR}/cds)
#find_package(TBB REQUIRED)
project(test)
include_directories(".")

add_executable(main main.cpp)
target_link_libraries(main cds)
#TBB::tbb

