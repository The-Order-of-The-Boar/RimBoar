assert __name__ != "__main__"

# builtin
from enum import Enum
from dataclasses import dataclass
from typing import List, Optional, Dict, Any
from pathlib import Path
import tomllib

# local
from utils import panic, is_file_executable, command_exists
from type_check import check_type


CONFIG_FILE_PATH: Path = Path("./cpmconfig.toml")
DEFAULT_BUILD_FOLDER: Path = Path("./build/")
TREE_PATH_VAR: str = "__TREE_PATH__"

class TargetType(Enum):
    Executable = 1
    Library = 2

@dataclass
class Target:
    name: str
    path: Path
    type: TargetType
    is_test: bool
    test_exclude_from_all: bool
    associated_test_target_name: Optional[str]

@dataclass
class ProjectConfig:
    targets: Dict[str, Target]
    build_types: List[str]
    build_folder: Path
    src_folder: Path
    main_target: Optional[str]
    default_build_type: Optional[str]
    cmake_custom_path: Optional[Path]

    def get_executable_path(self, target: str, build_type: str) -> Path:

        assert target in self.targets
        assert self.targets[target].type == TargetType.Executable
        assert build_type in self.build_types

        return Path(".", self.build_folder, build_type, self.targets[target].path, self.targets[target].name)
    
    def get_build_type_path(self, build_type: str) -> Path:

        assert build_type in self.build_types
        return Path(".", self.build_folder, build_type)

    def get_runnable_targets(self) -> List[str]:

        runnable: List[str] = []
        for target_name, target in self.targets.items():
            if target.type == TargetType.Executable:
                runnable.append(target_name)

        return runnable
    
    def get_testable_targets(self) -> List[str]:

        testable: List[str] = []
        for target_name, target in self.targets.items():
            if target.is_test or target.associated_test_target_name != None:
                testable.append(target_name)

        return testable
    
    def get_all_tests(self) -> List[str]:

        testable = self.get_testable_targets()

        for test in testable:
            if self.targets[test].test_exclude_from_all:
                testable.remove(test)

        return testable


def read_config() -> ProjectConfig:

    config = __parse_config_file()
    __validate_config(config)
    return config

def __parse_config_file() -> ProjectConfig:

    if not CONFIG_FILE_PATH.exists():
        panic(f"{CONFIG_FILE_PATH} not found")

    file = open(CONFIG_FILE_PATH, "rb")
    config_data = tomllib.load(file) 
    __insert_tree_path(config_data)

    project = __take(config_data, "project", Dict[str, Any])
    main_target: Optional[str] = __take_if(project, "main_target", str)
    build_types: List[str] = __take(project, "build_types", List[str])
    default_build_mode: Optional[str] = __take_if(project, "default_build_type", str)
    build_folder: str = __take_if_or_default(project, "build_folder", str, str(DEFAULT_BUILD_FOLDER))
    custom_cmake_path: Optional[str] = __take_if(project, "cmake_path", str)
    src_folder: str = __take(project, "src_folder", str)
    
    targets: Dict[str, Target] = dict()
    type TargetList = List[Dict[str, Any]]
    if project.get("targets", None) != None:
        
        executables: TargetList = __take_if_or_default(project["targets"], "executable", TargetList, [])
        for executable in executables:
            name: str = __take(executable, "name", str)
            path: str = __take(executable, "path", str)
            is_test: bool = __take_if_or_default(executable, "is_test", bool, False)
            associated_test_target_name: Optional[str] = __take_if(executable, "test_target", str) if not is_test else None
            exclude_from_all: bool = is_test and __take_if_or_default(executable, "exclude_from_all", bool, False)
            __reject_unused_keys(executable)

            if name in targets:
                panic("a target with name '{}' already exists")
        
            targets[name] = Target(name, Path(path), TargetType.Executable, is_test, exclude_from_all, associated_test_target_name)


        libraries: TargetList = __take_if_or_default(project["targets"], "library", TargetList, [])
        for library in libraries:
            name: str = __take(library, "name", str)
            path: str = __take(library, "path", str)
            associated_test_target_name: Optional[str] = __take_if(library, "test_target", str)
            __reject_unused_keys(library)
            
            if name in targets:
                panic("a target with name '{}' already exists")

            targets[name] = Target(name, Path(path), TargetType.Library, False, False, associated_test_target_name)
        
        __reject_unused_keys(project["targets"])
        project.pop("targets")

    __reject_unused_keys(project)
    __reject_unused_keys(config_data)

    return ProjectConfig(
        targets,
        build_types,
        Path(build_folder),
        Path(src_folder),
        main_target,
        default_build_mode,
        Path(custom_cmake_path) if custom_cmake_path != None else None
    )

def __validate_config(config: ProjectConfig):

    if not config.src_folder.exists():
        panic(f"{config.src_folder} does not exists, project.src_folder needs to be a existent directory")

    if not config.src_folder.is_dir():
        panic(f"{config.src_folder} is not a directory")

    if not config.build_folder.parent.exists():
        panic("project.build_folder parent path must exists")

    if config.main_target != None:
        
        if config.main_target not in config.targets:
            panic(f"'{config.main_target} target does not exists, project.main_target must be a existent target")

        if config.targets[config.main_target].type != TargetType.Executable:
            panic(f"main_target must be a executable target, {config.main_target}, is not")

    for build_type in config.build_types:
        if config.build_types.count(build_type) > 1:
            panic(f"repeated build type: {build_type}")

    if config.default_build_type != None:
        if config.default_build_type not in config.build_types:
            panic(f"{config.default_build_type} build type does not exists, project.default_build_type must be a existent build type")

    for target in config.targets.values():
        
        if target.associated_test_target_name != None:
            if not config.targets[target.associated_test_target_name].is_test:
                panic(f"associated test target must be a test, {target.associated_test_target_name} is not")
        
        if target.path.is_absolute():
            panic(f"target path must be relative, {target.path} is not")
        
    if config.cmake_custom_path != None:
        if not is_file_executable(config.cmake_custom_path):
            panic("project.cmake_custom_path must be a executable file")
    else:
        if not command_exists("cmake"):
            panic("cmake command not found")

def __reject_unused_keys(map: Dict[str, Any]):

    path = __take(map, TREE_PATH_VAR, str)

    for key in map.keys():
        panic(f"unrecognized key: {path}.{key}")

def __take_if[T](map: Dict[str, Any], key: str, vT: type[T]) -> Optional[T]:

    if (key not in map) or not check_type(map[key], vT):
        return None
    
    value: T = map[key]
    map.pop(key)
    return value


def __take_if_or_default[T](map: Dict[str, Any], key: str, vT: type[T], default: T) -> T:

    value = __take_if(map, key, vT)
    

    return (value if value != None else default)

def __take[T](map: Dict[str, Any], key: str, vT: type[T]) -> T:

    assert TREE_PATH_VAR in map, "internal error, the tree shold be annotated with tree path"

    if key not in map:
        panic(f"{map[TREE_PATH_VAR]}.{key} is required")

    if not check_type(map[key], vT):
        panic(f"{map[TREE_PATH_VAR]}.{key} needs to be an {T}")

    return map.pop(key)




def __insert_tree_path(map: Dict[str, Any], path: List[str] = []):

    def build_path_str(path: List[str]) -> str:
        return "".join("." + key for key in path)[1:]

    def dispatch(value: Any, path: List[str]):
        
        if check_type(value, Dict[str, Any]):
            handle_dict(value, path)
        elif check_type(value, List[Any]):
            handle_array(value, path)

    def handle_array(array: List[Any], path: List[str]):
        for idx, item in enumerate(array):
            dispatch(item, path + [str(idx)])

    def handle_dict(map: Dict[str, Any], path: List[str]):

        if TREE_PATH_VAR in map:
            panic(f"please, do not use the '{TREE_PATH_VAR}' key")

        for key, value in map.items():
            dispatch(value, path + [key])

        map[TREE_PATH_VAR] = build_path_str(path)

    return handle_dict(map, [])
