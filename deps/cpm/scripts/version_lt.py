#!/usr/bin/env python3

assert __name__ == "__main__"
from parse_versions import get_versions


v1, v2 = get_versions()

if v1 < v2:
    exit(0)
else:
    exit(1)
