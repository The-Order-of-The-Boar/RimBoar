#!/usr/bin/env bash
set -e

recursive_newer_than() {

    for node in $(find $1); do

        if [ $node -nt $2 ]; then
            return 0
        fi
    done

    return 1
}
