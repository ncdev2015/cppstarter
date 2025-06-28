cmake_minimum_required(VERSION 3.10)
project({{PROJECT_NAME}})

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")

add_executable({{PROJECT_NAME}} ${SRC_FILES})
