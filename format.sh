#!/usr/bin/env bash
set -e

find src -name '*.cpp' -or -name '*.hpp' | xargs clang-format -style=file -i
