cmake_minimum_required(VERSION 3.10)
project(ant-movement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-static")

add_executable(ant-movement main.cpp)