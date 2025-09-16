# hantek_drc

C library for reading Hantek `.drc` files

## Build

Build is powered by `cmake`
Just use:

```sh
cmake -S . -B build
cmake --build build
```

## Install

To build and install all variants (static/shared,debug/release) of library you can use following python script:

`./install.py` (requires Python 3.x)


## Usage

### CMake

To use library in cmake project you must install library and then just use:

```cmake
# set(HANTEK_DRC_USE_STATIC ON) # To use static version of library
find_package(hantek_drc REQUIRED)
target_link_libraries(your_program PRIVATE hantek_drc)
````

### Quick start

To read `.drc` you need to create `hantek_drc_info` struct:

```c
#include <hantek_drc/info.h>

bool read_drc(const char* path) {
    hantek_drc_info info = {
        .caps = hantek_drc_6254bd(),
        .handler = <handler>
    };
    bool success = hantek_drc_read_file(path, &info);
    success &= hantek_drc_info_finish_free(&info);
    return success;
}
```

### Examples

Check [examples](./examples/) directory