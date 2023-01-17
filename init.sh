#!/usr/bin/env bash

# install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh --disableMetrics
