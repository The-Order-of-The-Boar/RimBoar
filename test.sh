#!/usr/bin/env bash
set -e

source scripts/recursive_newer_than.sh

test_build_type="Debug"

if !([ -e "build" ]) || [ $test_build_type != "$(cat build/BUILD_TYPE)" ] || (recursive_newer_than src build/test_rimboar); then
    ./build.sh $test_build_type tests
    touch build/test_rimboar
fi

./build/test_rimboar
