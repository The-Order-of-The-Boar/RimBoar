#!/usr/bin/env bash
set -e


if !([ -e "vcpkg" ]); then
    echo "uninitialized project"
    exit 1
fi

if (($# < 2)) || (($# > 2)); then
    echo "invalid number of parameters, usage: build.sh <build type> <target>"
    exit 1
fi

build_type=$1
target=$2

if [ $build_type != "Debug" ] && [ $build_type != "Release" ] && [ $build_type != "Sanitize" ] && [ $build_type != "Profile" ]; then
    echo "invalid build mode"
    exit 1
fi

if [ $target != 'tests' ] && [ $target != 'main' ]; then
    echo "invalid target"
    exit 1
fi

case $target in
    "tests") target="test_rimboar";;
    "main")  target="rimboar";;
    *)
        echo "internal error, invalid target"
        exit 1
        ;;
esac


cmake -B build -S . -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build -j`getconf _NPROCESSORS_ONLN` --target rimboar_int $target
echo -n $build_type > build/BUILD_TYPE

cp build/rimboar .
