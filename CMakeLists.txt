cmake_minimum_required(VERSION 3.20)
project(blueprint)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES 
    src/main.cpp
)

set(INSTALLER_SOURCES 
    src/installer.cpp
)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXE_LINKER_FLAGS "--static")
endif()

add_executable(blueprint ${SOURCES})

add_executable(blueprint_installer ${INSTALLER_SOURCES})
