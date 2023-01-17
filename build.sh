#!/usr/bin/bash

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build -j`nproc`

cp build/rimboar .
