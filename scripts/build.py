#!/usr/bin/env python3

import argparse
import shutil
import os

from utils import cmd, panic, TARGET_MAP, BUILD_MODES


INTERNAL_TARGET = "rimboar_int"


def build(target: str, mode: str):

    if not os.path.exists("build"):
        os.mkdir("build")

    build_path = "build/" + mode

    if not os.path.exists(build_path):
        os.mkdir(build_path)

    if target not in TARGET_MAP:
        panic("invalid target, valid targets:", *[target in TARGET_MAP.keys()])

    if mode not in BUILD_MODES:
        panic("invalid build mode, valid modes:", *BUILD_MODES)

    cmake_target = TARGET_MAP[target]
    cpu_count = cmd("getconf _NPROCESSORS_ONLN", get_stdout=True).decode("utf-8").strip()

    print(f"build target: {target} -> {cmake_target}")
    print(f"build mode: {mode}")
    print(f"parallel units: {cpu_count}")

    cmd(f"cmake -B {build_path} -S . -DCMAKE_BUILD_TYPE={mode} -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake")
    cmd(f"cmake --build {build_path} --target {INTERNAL_TARGET} {cmake_target} -j{cpu_count}")
    shutil.copyfile(f"{build_path}/compile_commands.json", "compile_commands.json")


if __name__ == "__main__":

    parser = argparse.ArgumentParser(prog="build")
    parser.add_argument("target", type=str)
    parser.add_argument("mode", type=str)

    args = parser.parse_args()
    target = args.target.lower()
    mode = args.mode.lower()

    build(target, mode)
