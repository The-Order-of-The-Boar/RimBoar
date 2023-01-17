
# cmake config
cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project config
project(rimboar CXX)
add_executable(rimboar src/main.cpp)

# dependencies


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
    
    -std=c++17
    -pedantic -Wall -Wextra
    -fdiagnostics-color=always

    $<$<CONFIG:DEBUG,PROFILE,SANITIZE>:-g>

    $<$<CONFIG:RELEASE,PROFILE,SANITIZE>:-DNDEBUG>
    $<$<CONFIG:RELEASE,PROFILE,SANITIZE>:-O0>
    $<$<CONFIG:RELEASE,PROFILE>:-flto>
    $<$<CONFIG:RELEASE,PROFILE>:-march=native>
)
