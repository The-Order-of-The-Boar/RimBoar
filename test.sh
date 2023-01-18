#!/usr/bin/env bash
set -e

test_build_type="Debug"

if !([ -e "build" ]) || [ $test_build_type != "$(cat build/BUILD_TYPE)" ] || [ "build/test_rimboar" -ot "src" ]; then
    ./build.sh $test_build_type tests
fi

./build/test_rimboar
