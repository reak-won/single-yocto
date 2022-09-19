project(SEC)

cmake_minimum_required(VERSION 3.4.3)

file(GLOB SRCS src/*.c src/*.cpp)

add_executable(secd ${SRCS})

target_link_libraries(secd secapi)

#To enable #include "sec-api.h" or #include <sec-api.h>
target_include_directories(secd PRIVATE ${CMAKE_SOURCE_DIR}/sec-api/inc)

#install(TARGETS secd DESTINATION bin)
