#!/usr/bin/env python3

import itertools
import os
import sys

variants = [
    { "shared": "-DBUILD_SHARED_LIBS=ON", "static": "-DBUILD_SHARED_LIBS=OFF" },
    { "debug": "-DCMAKE_BUILD_TYPE=Debug", "release": "-DCMAKE_BUILD_TYPE=Release" }
]

for keys in itertools.product(*variants):
    options = " ".join(m[keys[i]] for i, m in enumerate(variants))
    build_dir = f"build/{"_".join(keys)}"
    cmd = ( 
        f"cmake -S . -B {build_dir} -DBUILD_TESTING=OFF {options} && "
        f"cmake --build {build_dir} && "
        f"{"sudo " if os.name == "posix" else ""}cmake --install {build_dir}"
    )
    if os.system(cmd) != 0:
        sys.exit(-1)
