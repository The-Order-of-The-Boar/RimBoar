#!/usr/bin/env python3

import argparse
import os
import sys

sys.path.append("scripts")
from build import build
from utils import panic, any_newer_than, BUILD_MODES, cmd
sys.path.pop()


parser = argparse.ArgumentParser(prog="run")
parser.add_argument("build_mode", type=str, nargs='?')
args = parser.parse_args()

build_mode = (args.build_mode if args.build_mode != None else "Debug").lower()

if build_mode not in BUILD_MODES:
    panic("invalid build mode, valid build modes:", *BUILD_MODES)

program_path = f"build/{build_mode}/rimboar"
if not os.path.exists(program_path) or any_newer_than(program_path, "src", "CMakeLists.txt", "run.py",  "scripts"):
    build("main", build_mode)
    cmd(f"touch {program_path}")

cmd(program_path)
