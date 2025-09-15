cmake_minimum_required(VERSION 4.1)
project(hantek_drc_csv VERSION 1.0.0 LANGUAGES C)

set(SRC main.c)
add_executable(hantek_drc_csv ${SRC})
target_link_libraries(hantek_drc_csv PRIVATE hantek_drc)