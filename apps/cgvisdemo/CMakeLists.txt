cmake_minimum_required(VERSION 3.16)

project(cgvisdemo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(cgvisdemo
  Demo.cpp
  Main.cpp
  MainWindow.cpp
)

target_link_libraries(cgvisdemo cgvis)

set_target_properties(cgvisdemo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
