#!/usr/bin/env bash

# setup errors
set -eu

SCRIPT_HOME=$(dirname -- "$( readlink -f -- "$0"; )";)
source $SCRIPT_HOME/vars.sh


rm -rf $POETRY_HOME
rm -rf $PYTHON_HOME
rm -rf $PYINSTALLER_HOME
rm -rf .venv
# It is not necessary to remove CPM_HOME as it is located inside PYINSTALLER_HOME