cmake_minimum_required(VERSION 4.0)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c
        functions.c
        parser.c
        functions.h
        parser.h
        proverki_operacii.c
        proverki_operacii.h
)
