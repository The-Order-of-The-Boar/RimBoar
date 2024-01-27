# this file do not should be imported
assert __name__ == "__main__"

# requires a minimum python version
import sys

MINIMUM_PYTHON_VERSION = (3, 12)
assert sys.version_info >= MINIMUM_PYTHON_VERSION, f"minimum python version is {MINIMUM_PYTHON_VERSION[0]}.{MINIMUM_PYTHON_VERSION[1]}"

import os
# permite rodar apenas em sistemas linux
assert os.name == 'posix', 'can only run on posix systems'

# builtin
import argparse
from typing import Optional, List
import importlib.metadata

# local
from config_file import read_config, ProjectConfig
from utils import panic
import operations



def build(config: ProjectConfig, args: argparse.Namespace, fw_args: Optional[List[str]]):
    
    if fw_args != None:
        panic("build command do not receives forward arguments")

    operations.build(config, args.target, args.build_type)

def run(config: ProjectConfig, args: argparse.Namespace, fw_args: Optional[List[str]]):
    
    operations.run(config, args.target, args.build_type, fw_args if fw_args != None else [])

def test(config: ProjectConfig, args: argparse.Namespace, fw_args: Optional[List[str]]):

    if fw_args != None and len(args.target) > 1:
        panic("test command only support forward arguments when testing a single target")

    target: str
    for target in args.target:
        operations.run(config, target, args.build_type, [])

def reload(config: ProjectConfig, args: argparse.Namespace, fw_args: Optional[List[str]]):
    
    if fw_args != None:
        panic("reload command do not receives forward arguments")

    operations.reload(config)

def clean(config: ProjectConfig, args: argparse.Namespace, fw_args: Optional[List[str]]):
    
    if fw_args != None:
        panic("clean command do not receives forward arguments")

    operations.clean(config)


def build_parser(config: ProjectConfig) -> argparse.ArgumentParser:

    parser = argparse.ArgumentParser(
        description="Provides a ergonomic CLI interface for cmake projects",
        epilog="repository: https://github.com/gabrielcfvg/cpm"
    )
    commands = parser.add_subparsers(title="commands", required=True)

    # version
    parser.add_argument(
        "-v", "--version",
        action="version",
        version=importlib.metadata.version("cpm")
    )

    # build
    build_command = commands.add_parser(
        name="build",
        description="build the specified cmake target",

    )
    build_command.add_argument(
        "-t", "--target",
        dest="target",
        default=config.main_target,
        nargs="+",
        choices=config.targets.keys(),
        help=f"the targets to be built, default: {config.main_target}"
    )
    build_command.add_argument(
        "-m", "--build_type",
        dest="build_type",
        default=config.default_build_type,
        choices=config.build_types,
        help=f"the build type to be used for building the targets, default: {config.default_build_type}"
    )
    build_command.set_defaults(func=build)

    # run
    run_command = commands.add_parser(
        name="run",
        description="run the specified cmake target, building it before if needed"
    )
    run_command.add_argument(
        "-t", "--target",
        dest="target",
        default=config.main_target,
        choices=config.get_runnable_targets(),
        help=f"the target to be runned, default: {config.main_target}"
    )
    run_command.add_argument(
        "-m", "--build_type",
        dest="build_type",
        default=config.default_build_type,
        choices=config.build_types,
        help=f"the build type to be used for building the target, default: {config.default_build_type}"
    )
    run_command.set_defaults(func=run)

    # test
    test_command = commands.add_parser(
        name="test",
        description="test the specified cmake target, or its associated test target, building it before if needed"
    )
    test_command.add_argument(
        "-t", "--target",
        dest="target",
        default=config.get_all_tests(),
        choices=config.get_testable_targets(),
        nargs="+",
        help=f"the targets to be tested, default: {config.get_all_tests()}"
    )
    test_command.add_argument(
        "-m", "--build_type",
        dest="build_type",
        default=config.default_build_type,
        choices=config.build_types,
        help=f"the build type to be used for building the targets, default: {config.default_build_type}"
    )
    test_command.set_defaults(func=test)

    # reload
    reload_command = commands.add_parser(
        name="reload",
        description="reload the cmake files"
    )
    reload_command.set_defaults(func=reload)

    # clean
    clean_command = commands.add_parser(
        name="clean",
        description="clean the build folder"
    )
    clean_command.set_defaults(func=clean)


    return parser



escaped_args: Optional[List[str]] = None
for idx, arg in enumerate(sys.argv):
    if arg == "--":
        escaped_args = sys.argv[idx + 1:]
        sys.argv = sys.argv[:idx]

config = read_config()
parser = build_parser(config)
args = parser.parse_args()
args.func(config, args, escaped_args)
