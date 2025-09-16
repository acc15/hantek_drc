cmake_minimum_required(VERSION 4.1)
project(hantek_drc VERSION 1.0.0 LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE) # Make it mandatory
set(CMAKE_C_CPPCHECK "cppcheck;--check-level=exhaustive")
set(CMAKE_C_CLANG_TIDY clang-tidy)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

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
     )
else()
     add_compile_options(-fms-extensions -Wno-microsoft-anon-tag -Wall -Wextra -Wpedantic -Werror)
endif()

set(CMAKE_DEBUG_POSTFIX "d")


include_directories(include)
set(HEADERS 
    include/hantek_drc/fwd.h
    include/hantek_drc/coupling.h
    include/hantek_drc/caps.h
    include/hantek_drc/channel.h
    include/hantek_drc/format.h
    include/hantek_drc/value.h
    include/hantek_drc/info.h
    include/hantek_drc/read.h
    include/hantek_drc/csv.h
    include/hantek_drc/mem.h
    include/hantek_drc/handler.h
    include/hantek_drc/filter.h
    include/hantek_drc/frame_handler.h
    include/hantek_drc/range.h
    include/hantek_drc/gnuplot.h
)

add_library(hantek_drc 
    src/caps.c
    src/read.c
    src/value.c
    src/channel.c
    src/format.c
    src/info.c
    src/csv.c
    src/mem.c
    src/handler.c
    src/filter.c
    src/frame_handler.c
    src/range.c
    src/gnuplot.c
)
target_sources(hantek_drc PUBLIC FILE_SET HEADERS BASE_DIRS include/hantek_drc FILES ${HEADERS})

include(CTest)
if(BUILD_TESTING)
    add_executable(hantek_drc_test 
        test/main.c    
        test/value.c
        test/read.c
        test/csv.c
        test/mem.c
        test/range.c
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
    # Ignore warning for check unit testing to work
    target_compile_options(hantek_drc_test PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
    add_test(NAME hantek_drc_test COMMAND hantek_drc_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples/gnuplot)
    add_subdirectory(examples/csv)
endif()

if(NOT EXPORT_NAME)
    if(BUILD_SHARED_LIBS)
        set(EXPORT_NAME ${PROJECT_NAME}_shared)
    else()
        set(EXPORT_NAME ${PROJECT_NAME}_static)
    endif()
endif()

include(GNUInstallDirs)
install(
    TARGETS hantek_drc DESTINATION ${CMAKE_INSTALL_LIBDIR}/hantek_drc 
    EXPORT ${EXPORT_NAME}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hantek_drc
)
install(
    EXPORT ${EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hantek_drc
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    hantek_drc-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/hantek_drc-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hantek_drc
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/hantek_drc-version.cmake 
    COMPATIBILITY SameMajorVersion 
)
install(
    FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/hantek_drc-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/hantek_drc-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hantek_drc
)