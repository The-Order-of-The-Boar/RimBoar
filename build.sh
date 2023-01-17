#!/usr/bin/env bash

set -e

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build -j`getconf _NPROCESSORS_ONLN`

cp build/rimboar .
