import os
import subprocess
import sys
from pathlib import Path
from typing import Optional


BUILD_MODES = ("debug", "release", "profile", "test_release")
TARGET_MAP = {"main": "rimboar", "tests": "test_rimboar"}


def cmd(command: str, get_stdout: bool = False, env=None) -> Optional[bytes]:

    if get_stdout:
        return subprocess.run(command.split(' '), check=True, capture_output=True, env=env).stdout
    else:
        subprocess.run(command.split(' '), check=True, stdout=sys.stdout)


def panic(*args):

    print("ERROR:", *args)
    exit(1)


def any_newer_than(target: str, *nodes: str) -> bool:

    node = Path(nodes[0])
    target_date = Path(target).stat().st_mtime

    if node.stat().st_mtime > target_date:
        return True

    if node.is_dir():
        for children in node.iterdir():
            if any_newer_than(target, str(children)) is True:
                return True

    if len(nodes) > 1:
        return any_newer_than(target, *nodes[1:])
    else:
        return False


def get_executable_path(build_mode: str, target: str) -> str:

    assert build_mode in BUILD_MODES
    assert target in TARGET_MAP

    return f"build/{build_mode}/{TARGET_MAP[target]}"
