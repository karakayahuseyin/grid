# Copyright (c) 2025 Hüseyin Karakaya (https://github.com/karakayahuseyin)
# Licensed under the MIT License. Part of the Grid project.

cmake_minimum_required(VERSION 3.10)

project(
  grid-web-server
  VERSION 0.1.0
  LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wconversion
        -Wshadow
    )
endif()

find_package(Threads REQUIRED)

add_library(libgrid
  src/Socket.cc
  src/ThreadPool.cc
)

target_include_directories(libgrid PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(libgrid PUBLIC Threads::Threads)

# Example executable
add_executable(grid examples/main.cc)
target_link_libraries(grid PRIVATE libgrid Threads::Threads)