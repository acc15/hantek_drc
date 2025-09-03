cmake_minimum_required(VERSION 4.1)
project(hantek_drc LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE) # Make it mandatory
add_library(hantek_drc 
    src/file.c
    src/value.c
    src/info.c
    src/csv.c
)

find_package(Check)
if(${Check_FOUND})
    add_executable(hantek_drc_test 
        test/main.c    
        test/value.c
        test/file.c
        test/csv.c
    )
    target_link_libraries(hantek_drc_test PRIVATE hantek_drc Check::checkShared)
    
    enable_testing()
    add_test(NAME hantek_drc_test COMMAND hantek_drc_test)
endif()
