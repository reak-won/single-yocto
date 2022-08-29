project(SEC)

cmake_minimum_required(VERSION 3.4.3)


message(WARNING ${PROJECT_SOURCE_DIR})
message(WARNING ${PROJECT_BINARY_DIR})

file(GLOB SRCS src/*.cpp)
add_executable(sec-test ${SRCS})

target_link_libraries(sec-test secapi)
target_include_directories(sec-test PRIVATE ${CMAKE_SOURCE_DIR}/sec-api/inc)
