project(SEC)

cmake_minimum_required(VERSION 3.4.3)

FILE(GLOB SRCS src/*.cpp src/*.c)

add_executable(sec-core ${SRCS})

