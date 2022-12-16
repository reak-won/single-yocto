project(SEC)

cmake_minimum_required(VERSION 3.4.3)

FILE(GLOB SRCS src/*.cpp src/*.c ${CMAKE_SOURCE_DIR}/comm/src/*.cpp)

add_executable(sec-core ${SRCS})
target_link_libraries(sec-core crypto)

#use the header files like placing current directories
target_include_directories(sec-core PRIVATE ${CMAKE_SOURCE_DIR}/public
				${CMAKE_SOURCE_DIR}/comm/inc)
