assert __name__ != "__main__"

import sys

assert len(sys.argv) == 3

v1 = [int(vn) for vn in sys.argv[1].split('.')]
assert len(v1) == 3, "first parameter is not a valid version"

v2 = [int(vn) for vn in sys.argv[2].split('.')]
assert len(v2) == 3, "second parameter is not a valid version"

def get_versions():
    return v1, v2
