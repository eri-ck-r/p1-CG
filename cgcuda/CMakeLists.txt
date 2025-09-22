cmake_minimum_required(VERSION 3.6)

project(cgcuda CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cgcuda STATIC
  src/CUDAHelper.cpp
)

target_include_directories(cgcuda PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(cgcuda cg)
