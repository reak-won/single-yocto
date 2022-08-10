project(SEC)

cmake_minimum_required(VERSION 3.4.3)

file(GLOB SRCS src/*.c src/*.cpp)

add_executable(sec ${SRCS})

install(TARGETS sec DESTINATION bin)
