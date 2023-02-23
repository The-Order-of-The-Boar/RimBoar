#!/usr/bin/env bash
set -e

source scripts/any_newer_than.sh

build_type="Profile"

if !([ -e "build" ]) || [ $build_type != "$(cat build/BUILD_TYPE)" ] || (any_newer_than build/rimboar src CMakeLists.txt build.sh run.sh); then
    ./build.sh $build_type main
    touch build/rimboar
fi

#./build/rimboar
CPUPROFILE=boar.prof ./build/rimboar
google-pprof --web ./build/rimboar boar.prof
