cmake_minimum_required(VERSION 3.5)

project(pipe)

set(
    SOURCES
    ./pipe1.c
)

add_executable(${PROJECT_NAME} ${SOURCES})