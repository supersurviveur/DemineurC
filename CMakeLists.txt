cmake_minimum_required(VERSION 3.24)
project(DemineurC)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(DemineurC
        display.c core.c generation.c
        main.c)