#!/usr/bin/env bash
set -e

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

if !([ -e "build" ]) || [ $build_type != "$(cat build/BUILD_TYPE)" ] || [ "build/rimboar" -ot "src" ]; then
    ./build.sh $build_type main
fi

./build/rimboar
