#!/usr/bin/env bash
set -e

source scripts/recursive_newer_than.sh

build_type="Debug"

if (($# > 0));
then

    if (($# > 1));
    then
        echo "invalid number of parameters"
        exit 1
    fi

    build_type=$1
fi

if !([ -e "build" ]) || [ $build_type != "$(cat build/BUILD_TYPE)" ] || (recursive_newer_than src build/rimboar); then
    ./build.sh $build_type main
    touch build/rimboar
fi

./build/rimboar
