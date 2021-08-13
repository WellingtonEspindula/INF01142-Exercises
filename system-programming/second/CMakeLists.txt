cmake_minimum_required(VERSION 3.20)
project(SISOP C)

set(CMAKE_C_STANDARD 11)

add_executable(SISOP main.c bytearray.h bytearray.c string.h string.c arraylist.h arraylist.c)
