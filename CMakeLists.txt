cmake_minimum_required(VERSION 4.1)
project(hantek_drc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE) # Make it mandatory
set(CMAKE_DEBUG_POSTFIX "d")

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

set(SRC 
    src/caps.cpp
    src/file.cpp
    src/info.cpp
    src/value.cpp
)

set(HEADERS 
    src/pow.hpp
    src/binary.hpp
    src/caps.hpp
    src/channel.hpp
    src/coupling.hpp
    src/file.hpp
    src/info.hpp
    src/value.hpp
)

add_library(hantek_drc ${SRC})
target_sources(hantek_drc PUBLIC FILE_SET HEADERS BASE_DIRS src FILES ${HEADERS})

include(CTest)
if (BUILD_TESTING)
    find_package(doctest REQUIRED)

    add_executable(hantek_drc_test 
        test/main.cpp
        test/pow.cpp
        test/value.cpp
        test/caps.cpp
        test/info.cpp
    )
    target_link_libraries(hantek_drc_test PRIVATE hantek_drc doctest::doctest)
    add_test(NAME hantek_drc_test COMMAND hantek_drc_test)
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