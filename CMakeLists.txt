cmake_minimum_required(VERSION 3.26)
project(DemineurC C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(DemineurC
        display.c)
