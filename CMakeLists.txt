cmake_minimum_required(VERSION 3.0.0)
project(MeshVolume)

set(CMAKE_CXX_STANDARD 11)
if (NOT MSVC)
  set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -O2 -std=c++11")
endif()

add_subdirectory(src)
