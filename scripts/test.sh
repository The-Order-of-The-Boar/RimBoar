#!/usr/bin/env bash
set -e

source scripts/any_newer_than.sh

test_build_type="Debug"

if !([ -e "build" ]) \
    || [ $test_build_type != "$(cat build/BUILD_TYPE)" ] \
    || (any_newer_than build/test_rimboar src CMakeLists.txt build.sh test.sh); then
    ./scripts/build.sh $test_build_type tests
    > build/test_rimboar
fi

./build/test_rimboar
