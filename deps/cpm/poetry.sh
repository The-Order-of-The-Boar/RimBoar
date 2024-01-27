#!/usr/bin/env bash

# setup errors
set -eu

# imports
SCRIPT_HOME=$(dirname -- "$( readlink -f -- "$0"; )";)
source $SCRIPT_HOME/vars.sh


# call setup_env.sh to ensure that all dependencies are meet
$SCRIPT_HOME/setup_env.sh > /dev/null


$POETRY_BIN -C $SCRIPT_HOME "$@"