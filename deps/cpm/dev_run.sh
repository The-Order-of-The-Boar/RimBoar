#!/usr/bin/env bash

# setup errors
set -eu

# imports
SCRIPT_HOME=$(dirname -- "$( readlink -f -- "$0"; )";)
source $SCRIPT_HOME/vars.sh


cd $SCRIPT_HOME/example_project
$SCRIPT_HOME/poetry.sh run python3 $SCRIPT_HOME/cpm "$@"
