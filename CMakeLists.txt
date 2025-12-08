cmake_minimum_required(VERSION 4.0)
project(Valt C)

set(CMAKE_C_STANDARD 99)

add_executable(Valt
        src/main.c
        src/inc/valt.h
        src/inc/utils.h
        src/utils.c
        src/inc/token.h
        src/token.c
        src/inc/lexer.h
        src/lexer.c
)
