#!/usr/bin/env python3

import os
from build import build
from utils import panic, any_newer_than, BUILD_MODES, cmd, get_executable_path as _get_executable_path
from subprocess import CalledProcessError


get_executable_path = lambda build_mode: _get_executable_path(build_mode, "tests")


def build_test(build_mode: str):

    build_mode = build_mode.lower()
    assert build_mode in BUILD_MODES

    program_path = get_executable_path(build_mode)
    if not os.path.exists(program_path) or any_newer_than(program_path, "src", "CMakeLists.txt", "scripts/test.py",  "scripts"):
        build("tests", build_mode)
        cmd(f"touch {program_path}")


build_test("debug")
build_test("test_release")

try:
    cmd(get_executable_path("debug"))
    cmd(get_executable_path("test_release"))
except CalledProcessError:
    pass
