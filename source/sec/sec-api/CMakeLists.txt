project (SEC)

cmake_minimum_required(VERSION 3.4.3)

file(GLOB SRCS ./src/*.cpp)
add_library(secapie SHARED ${SRCS})
