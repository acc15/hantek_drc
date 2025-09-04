cmake_minimum_required(VERSION 4.1)
project(hantek_drc LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE) # Make it mandatory

if(MSVC)
     add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Disables CRT secure warning to allow cross-platform stdlib usage
     add_compile_options(
        /W4 # Enable informational warnings (1-4 level)
        /WX # Treat warnings as errors
        /utf-8 # Enable UTF-8 support
        /Zc:wchar_t # wchar_t as builtin type
        /Zc:forScope # Removes possibility to use out of scope loop variables
        # Removes unreferenced data or functions that are COMDATs, or that only have internal linkage. 
        # Greatly reduces binary size
        /Zc:inline
        # Enables correct __cplusplus version reporting to enable new stdlib++ features
        /Zc:__cplusplus
     )
else()
     add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_library(hantek_drc 
    src/read.c
    src/value.c
    src/info.c
    src/csv.c
    src/mem.c
)

include(CTest)
if (BUILD_TESTING)

    add_executable(hantek_drc_test 
        test/main.c    
        test/value.c
        test/read.c
        test/csv.c
        test/mem.c
    )
    target_link_libraries(hantek_drc_test PRIVATE hantek_drc)

    find_package(Check QUIET)
    if(Check_FOUND)
        target_link_libraries(hantek_drc_test PRIVATE Check::checkShared)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(Check REQUIRED Check)
        target_include_directories(hantek_drc_test PRIVATE ${Check_INCLUDE_DIRS})
        target_link_libraries(hantek_drc_test PRIVATE ${Check_LINK_LIBRARIES})
    endif()
    target_compile_options(hantek_drc_test PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
    add_test(NAME hantek_drc_test COMMAND hantek_drc_test)
endif()
