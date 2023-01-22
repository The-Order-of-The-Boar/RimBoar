
# cmake config
cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# project config

project(rimboar CXX)

add_library(rimboar_int
    
    src/logging/source_location.cpp
    src/logging/log.cpp
    src/logging/assert.cpp

    src/graphics/graphic_manager.cpp
    src/graphics/imgui_handler.cpp
    src/graphics/sdl_handler.cpp

    src/scenes/game_application.cpp
    src/scenes/menu_scene.cpp
    src/scenes/game_scene.cpp

    src/utils/time_utils.cpp
)
target_include_directories(rimboar_int INTERFACE src)

add_executable(rimboar src/main/main.cpp)
target_link_libraries(rimboar PRIVATE rimboar_int)

add_executable(test_rimboar

    src/tests/test.cpp
    src/tests/slotmap.cpp
)
target_link_libraries(test_rimboar PRIVATE rimboar_int)


# dependencies

find_package(Catch2 REQUIRED)
target_link_libraries(test_rimboar PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

find_package(SDL2 REQUIRED)
target_link_libraries(rimboar_int PUBLIC SDL2::SDL2 SDL2::SDL2main)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(rimboar_int PUBLIC imgui::imgui)

find_package(fmt REQUIRED)
target_link_libraries(rimboar_int PUBLIC fmt::fmt)

find_package(glm REQUIRED)
target_link_libraries(rimboar_int PUBLIC glm::glm)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(rimboar_int PUBLIC ${STB_INCLUDE_DIRS})


# flags

if ((NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") AND (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    message(FATAL "unsupported compiler")
endif()

foreach(target IN ITEMS rimboar_int rimboar test_rimboar)
    
    target_link_libraries(${target} PRIVATE

        $<$<CONFIG:RELEASE,PROFILE>:-flto>

        $<$<CONFIG:SANITIZE>:-fsanitize=address>
        $<$<CONFIG:SANITIZE>:-fsanitize=undefined>
    )

    target_compile_options(${target} PRIVATE

        -std=c++20
        -pedantic -Wall -Wextra
        -fdiagnostics-color=always

        $<$<CONFIG:DEBUG,PROFILE,SANITIZE>:-g>

        $<$<CONFIG:RELEASE,PROFILE>:-DNDEBUG>
        $<$<CONFIG:RELEASE,PROFILE>:-O3>
        $<$<CONFIG:RELEASE,PROFILE>:-flto>
        $<$<CONFIG:RELEASE,PROFILE>:-march=native>
    )

endforeach()
