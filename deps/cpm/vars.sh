#!/usr/bin/env bash

# setup errors
set -eu


# setting the path variables
# TODO: considerar colocar o poetry, python e build do cpm num mesmo diretório

SCRIPT_HOME="$(dirname -- "$( readlink -f -- "$0"; )";)"

POETRY_HOME="$SCRIPT_HOME/poetry"
POETRY_BIN="$POETRY_HOME/bin/poetry"
POETRY_VERSION="1.7.1"
POETRY_VERSION_PATH="$POETRY_HOME/.poetry_version"

PYTHON_HOME="$SCRIPT_HOME/python"
PYTHON_BIN="$PYTHON_HOME/bin/python3"
PYTHON_VERSION="3.12.1"
PYTHON_REDUCED_VERSION=$(echo "$PYTHON_VERSION" | cut -d'.' -f1,2)
PYTHON_NAMES=("python3" "python$PYTHON_REDUCED_VERSION")
PYTHON_VERSION_PATH="$PYTHON_HOME/.python_version"
PYTHON_LOCAL_INSTALL_TYPE_PATH="$PYTHON_HOME/.install_type"
PYTHON_LOCAL_INSTALL_TYPE_LOCAL="local"
PYTHON_LOCAL_INSTALL_TYPE_SHORTCUT="shortcut"

PYINSTALLER_HOME="$SCRIPT_HOME/build" # prevents the root folder from getting too dirty
PYINSTALLER_WORKPATH="cache" # two consecutive build folders are confusing
PYINSTALLER_DIST="dist"

CPM_HOME="$PYINSTALLER_HOME/$PYINSTALLER_DIST/cpm"
CPM_BIN="$CPM_HOME/cpm"
CPM_VERSION="0.1.5"
CPM_VERSION_PATH="$CPM_HOME/.cpm_path"


# scripts
function assert_file_existence { if ! [[ -f $1 ]]; then echo "$1 not found"; exit 1; fi }
get_python_version="$SCRIPT_HOME/scripts/get_python_version.py"; assert_file_existence $get_python_version
version_gte="$SCRIPT_HOME/scripts/version_gte.py"; assert_file_existence $version_gte
version_lt="$SCRIPT_HOME/scripts/version_lt.py"; assert_file_existence $version_lt


# update linker path
set +u # removing the variable referencing restriction, because we need to access an external variable
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PYTHON_HOME/lib # the local python needs to find its shared libraries
set -u # adding back the restriction
