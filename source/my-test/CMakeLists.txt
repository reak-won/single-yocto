project(MY_TEST)

cmake_minimum_required(VERSION 3.4.3)

file(GLOB SRCS *.c)

add_executable(my-test ${SRCS})

install(TARGETS my-test DESTINATION bin)
