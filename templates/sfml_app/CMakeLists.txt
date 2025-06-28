cmake_minimum_required(VERSION 3.10)
project({{PROJECT_NAME}})

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")

add_executable({{PROJECT_NAME}} ${SRC_FILES})

target_link_libraries({{PROJECT_NAME}} sfml-graphics sfml-window sfml-system)
