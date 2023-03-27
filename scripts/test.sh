#!/usr/bin/env bash
set -e

source scripts/any_newer_than.sh

debug_test_build_type="Debug"
release_test_build_type="Test_Release"

if ! [ -e "build" ] || [ $debug_test_build_type != "$(cat build/BUILD_TYPE)" ] || (any_newer_than build/test_rimboar src CMakeLists.txt build.sh test.sh); then
    ./scripts/build.sh $debug_test_build_type tests
    touch build/test_rimboar
fi

./build/test_rimboar


if ! [ -e "build" ] || [ $release_test_build_type != "$(cat build/BUILD_TYPE)" ] || (any_newer_than build/test_rimboar src CMakeLists.txt build.sh test.sh); then
    ./scripts/build.sh $release_test_build_type tests
    touch build/test_rimboar
fi

./build/test_rimboar
