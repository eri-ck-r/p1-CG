cmake_minimum_required(VERSION 3.16)

project(cgvis CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cgvis STATIC
  src/BoxSource.cpp
  src/ConeSource.cpp
  src/CylinderSource.cpp
  src/Glyph3.cpp
  src/LookupTable.cpp
  src/Mapper.cpp
  src/PolyData.cpp
  src/PolyDataMapper.cpp
  src/PolyMesh.cpp
  src/PolyMeshBuilder.cpp
  src/PolyMeshMapper.cpp
  src/Scalars.cpp
  src/Scene.cpp
  src/SceneWindow.cpp
  src/Source.cpp
  src/SphereSource.cpp
  src/Transform.cpp
  src/TriCellMesh.cpp
  src/TriCell.cpp
  src/TriCellMeshMapper.cpp
  src/TriCellMeshNormals.cpp
  src/TriCellMeshReader.cpp
)

target_include_directories(cgvis PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/
)

target_link_libraries(cgvis cg)

set_target_properties(cgvis PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
