cmake_minimum_required(VERSION 3.1)
project(MyFirstC C)

set(CMAKE_C_STANDARD 99)

add_executable(MyFirstC main.c convert.h convert.c icon.res)
