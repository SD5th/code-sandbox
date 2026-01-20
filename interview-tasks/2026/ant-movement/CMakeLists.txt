cmake_minimum_required(VERSION 3.10)
project(ant-problem-solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-static")
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

set(SOURCES
    main.cpp
    source
    source/Config.h
    source/Config.cpp
    source/AntMatrix.h
    source/AntMatrix.cpp
    source/AntUtils.h
    source/AntUtils.cpp
    source/CanEnterCell.h
    source/CanEnterCell
    source/CanEnterCell/CanEnterCell.cpp
    source/CanEnterCell/CompareSumOfDigitsWithLimit.cpp
    source/CanEnterCell/CompareSumOfAbsNumbersWithLimit.cpp
    source/CanEnterCell/CompareSumOfSquaredNumbersWithLimit.cpp
)

add_executable(solve-ant-problem ${SOURCES})