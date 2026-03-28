cmake_minimum_required(VERSION 3.10)
project(oavi1laba)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  source/bmp_image.cpp
  main.cpp
)

add_executable(laba ${SOURCES})