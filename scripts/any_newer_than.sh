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

any_newer_than() {

    for ((idx = 2; idx <= $#; ++idx)); do
        if recursive_newer_than ${!idx} $1; then
            return 0
        fi
    done

    return 1
}
