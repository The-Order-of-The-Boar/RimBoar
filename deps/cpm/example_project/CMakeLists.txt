
# cmake config
cmake_minimum_required(VERSION 2.22.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# project config

project(cpm_example LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


# targets

add_subdirectory(src/main main)
add_subdirectory(src/core core)
add_subdirectory(src/utils utils)
add_subdirectory(src/tests tests)