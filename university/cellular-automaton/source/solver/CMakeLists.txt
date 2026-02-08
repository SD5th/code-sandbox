cmake_minimum_required(VERSION 3.10)
project(solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    solver.cpp
    rules.cpp
)

add_library(solver SHARED ${SOURCES})

set_target_properties(solver PROPERTIES OUTPUT_NAME "solver")