cmake_minimum_required(VERSION 3.24)
project(DemineurC)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(DemineurC
        main.c display.c core.c generation.c)
