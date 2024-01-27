#!/usr/bin/env bash

# setup errors
set -eu

# imports
SCRIPT_HOME=$(dirname -- "$( readlink -f -- "$0"; )";)
source $SCRIPT_HOME/vars.sh



if ! [[ -f $CPM_BIN ]]; then
    echo "the CPM executable was not found. run the build.sh script to build CPM"
    exit 1
fi

if $version_lt $( cat $CPM_VERSION_PATH ) $CPM_VERSION; then
    echo "the current CPM executable is outdated, run the build.sh script to build the newest version"
    exit 1
fi

$CPM_BIN "$@"
