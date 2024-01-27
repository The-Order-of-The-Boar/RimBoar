assert __name__ != "__main__"

from typing import get_origin, get_args, Any, Dict, List, Union, Optional, Callable, TypeGuard, TypeAliasType, cast
from types import UnionType
from utils import panic



def check_type[T](value: Any, tp: type[T]) -> TypeGuard[T]:

    if tp == Any:
        return True

    if isinstance(tp, TypeAliasType):
        return check_type(value, tp.__value__) 

    if get_origin(tp) == tp or get_origin(tp) == None:
        return isinstance(value, tp)

    return check_generic_type(value, tp)

def check_generic_type(value: Any, tp: type) -> bool:

    def check_multiple(value: Any, *tps: type) -> bool:

        assert isinstance(value, (Union, UnionType, Optional))

        return any(check_type(value, tp) for tp in tps)

    def check_dict(value: Any, tp: type) -> bool:

        assert isinstance(value, (Dict, dict))

        (key_tp, item_tp) = get_args(tp)
        value: Dict[Any, Any] = cast(Dict[Any, Any], value)

        def __old_fn(key_tp: type, item_tp: type):
            for key, item in value.items():
                if not (check_type(key, key_tp) and check_type(item, item_tp)):
                    return False
            return True

        r = all([(check_type(key, key_tp) and check_type(item, item_tp))
                 for key, item in value.items()])
        
        # roda isso ao menos uma vez so pra verificar
        assert r == __old_fn(key_tp, item_tp), 'runtime test failed sorry'
        return r


    def check_list(value: List[Any], tp: type) -> bool:
        
        assert isinstance(value, (List, list))

        item_tp = get_args(tp)[0]

        def __old_fn(item_tp: type):
            for item in value:
                if not check_type(item, item_tp):
                    return False
            return True
        
        r = all([check_type(item, item_tp) for item in value])
        assert r == __old_fn(item_tp), 'runtime test failed sorry'
        return r


    check_map: Dict[type, Callable[[Any, type], bool]] = {
        Dict: check_dict,
        dict: check_dict,
        List: check_list,
        list: check_list,
        Union: check_multiple,
        Optional: check_multiple,
        UnionType: check_multiple,
    }

    origin = get_origin(tp)
    assert origin != None

    if type(value) != origin:
        return False

    if origin in check_map:
        return check_map[origin](value, tp)
    else:
        panic(f"internal_error: unsurported generic type: {origin}")
