assert __name__ != "__main__"

# builtin
from pathlib import Path
import os
import shutil
from typing import List, Callable

# local
from config_file import ProjectConfig
from utils import any_newer_than, cmd



def __make_build_directory(config: ProjectConfig, build_type: str) -> bool:

    assert build_type in config.build_types

    if not (a := config.build_folder.exists()):
        os.mkdir(config.build_folder)

    path = Path(config.build_folder, build_type)
    if not (b := path.exists()):
        os.mkdir(path)

    return (not a) or (not b)

def __get_cmake_command(config: ProjectConfig) -> str:
    
    if config.cmake_custom_path != None:
        return f"./{config.cmake_custom_path}"
    else:
        return "cmake"

    

def build(config: ProjectConfig, target: str, build_type: str):

    assert target in config.targets
    assert build_type in config.build_types

    dir_created = __make_build_directory(config, build_type)

    cmake = __get_cmake_command(config)
    cpu_count = cmd("getconf _NPROCESSORS_ONLN", get_stdout=True).stdout.decode("utf-8").strip() # type: ignore
    build_folder = config.get_build_type_path(build_type)

    # TODO: filter ignored CMakeLists.txt's
    filter: Callable[[Path], bool] = lambda path: path.name == "CMakeLists.txt"
    if dir_created or any_newer_than(build_folder, Path(".", "CMakeLists.txt"), config.src_folder, filter=filter):
        print(f"reloading cmake cache in '{build_type}'") # TODO: colorir
        cmd(f"{cmake} -B {build_folder} -S . -DCMAKE_BUILD_TYPE={build_type}")

    print(f"building target '{target}' with '{build_type}'") # TODO: colorir
    cmd(f"{cmake} --build {build_folder} --target {target} -j{cpu_count}")
    
    commands_path = Path(build_folder, "compile_commands.json")
    if commands_path.exists():
        shutil.copyfile(commands_path, "compile_commands.json")

def run(config: ProjectConfig, target: str, build_type: str, args: List[str]):

    fargs = " ".join(args)
    build(config, target, build_type)
    executable_path = config.get_executable_path(target, build_type)
    # assert is_file_executable(executable_path) TODO: remove comment
    
    print(f"running '{target}'") # TODO: colorir


    exit_code = cmd(f"./{executable_path} {fargs}", check=False).exit_code
    if exit_code != 0:
        print(f"target exited with code {exit_code}") # colorir

def reload(config: ProjectConfig):
    
    cmake = __get_cmake_command(config)
    for build_type in config.build_types:
        __make_build_directory(config, build_type)
        cmd(f"{cmake} -B {config.get_build_type_path(build_type)} -S . -DCMAKE_BUILD_TYPE={build_type}")

def clean(config: ProjectConfig):
    shutil.rmtree(config.build_folder)
    