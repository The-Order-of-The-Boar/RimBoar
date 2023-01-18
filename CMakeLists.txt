
# cmake config
cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# project config

project(rimboar CXX)

add_library(rimboar_int src/__placeholder__.cpp)
target_include_directories(rimboar_int INTERFACE src)

add_executable(rimboar src/main/main.cpp)
target_link_libraries(rimboar PRIVATE rimboar_int)

add_executable(test_rimboar src/tests/test.cpp)
target_link_libraries(test_rimboar PRIVATE rimboar_int)


# dependencies

find_package(Catch2 REQUIRED)
target_link_libraries(test_rimboar PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)


# flags

if ((NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") AND (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    message(FATAL "unsupported compiler")
endif()

target_link_libraries(rimboar PRIVATE

    $<$<CONFIG:RELEASE,PROFILE>:-flto>

    $<$<CONFIG:SANITIZE>:-fsanitize=address>
    $<$<CONFIG:SANITIZE>:-fsanitize=undefined>
)
    
target_compile_options(rimboar PRIVATE
    
    -std=c++20
    -pedantic -Wall -Wextra
    -fdiagnostics-color=always

    $<$<CONFIG:DEBUG,PROFILE,SANITIZE>:-g>

    $<$<CONFIG:RELEASE,PROFILE>:-DNDEBUG>
    $<$<CONFIG:RELEASE,PROFILE>:-O3>
    $<$<CONFIG:RELEASE,PROFILE>:-flto>
    $<$<CONFIG:RELEASE,PROFILE>:-march=native>
)
