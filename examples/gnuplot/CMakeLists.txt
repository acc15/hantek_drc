cmake_minimum_required(VERSION 4.1)
project(hantek_drc_gnuplot VERSION 1.0.0 LANGUAGES C)

set(SRC main.c)
add_executable(hantek_drc_gnuplot ${SRC})
target_link_libraries(hantek_drc_gnuplot PRIVATE hantek_drc)