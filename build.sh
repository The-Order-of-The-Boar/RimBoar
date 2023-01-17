#!/usr/bin/env bash

set -e


if (($# > 0));
then

    if (($# > 1));
    then
        echo "invalid number of parameters"
        exit 1
    fi

    if [ $1 != "Debug" ] && [ $1 != "Release" ] && [ $1 != "Sanitize" ] && [ $1 != "Profile" ];
    then
        echo "invalid build mode"
        exit 1
    fi

    cmake -B build -S . -DCMAKE_BUILD_TYPE=$1 -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
else
    cmake -B build -S . -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
fi

cmake --build build -j`getconf _NPROCESSORS_ONLN`

cp build/rimboar .
