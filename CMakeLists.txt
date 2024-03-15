cmake_minimum_required(VERSION 3.28)
project(blueprint)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES 
    src/main.cpp
)

add_executable(blueprint ${SOURCES})