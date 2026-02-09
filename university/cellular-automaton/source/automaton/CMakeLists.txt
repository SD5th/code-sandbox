cmake_minimum_required(VERSION 3.10)
project(automaton)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    automaton.cpp
    interface.cpp
)

add_library(automaton SHARED ${SOURCES})

if(OpenMP_CXX_FOUND)
    target_link_libraries(automaton PRIVATE OpenMP::OpenMP_CXX)
endif()

set_target_properties(automaton PROPERTIES OUTPUT_NAME "automaton")