cmake_minimum_required(VERSION 3.10)
project(automaton)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_OPENMP "Enable OpenMP parallelization" ON)
option(USE_NEIGHBOURS "Use precomputed neighbours array" OFF)

message(STATUS "Configuration:")
message(STATUS "  USE_OPENMP: ${USE_OPENMP}")
message(STATUS "  USE_NEIGHBOURS: ${USE_NEIGHBOURS}")

if(USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_FOUND)
        message(STATUS "OpenMP found: ${OpenMP_CXX_VERSION}")
        set(OPENMP_FLAGS ${OpenMP_CXX_FLAGS})
        add_compile_definitions(_OPENMP)
        set(OPENMP_LIBS ${OpenMP_CXX_LIBRARIES})
    else()
        message(WARNING "OpenMP not found, building without parallelization")
        set(USE_OPENMP OFF)
    endif()
else()
    message(STATUS "Building without OpenMP (disabled by option)")
endif()

if(USE_NEIGHBOURS)
    #add_compile_definitions(USE_NEIGHBOURS=1)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SOURCES
    automaton.cpp
    interface.cpp
)

add_library(automaton SHARED ${SOURCES})

if(USE_OPENMP AND OPENMP_FOUND)
    target_compile_options(automaton PRIVATE ${OPENMP_FLAGS})
    target_link_libraries(automaton PRIVATE ${OPENMP_LIBS})
endif()

set_target_properties(automaton PROPERTIES OUTPUT_NAME "automaton")