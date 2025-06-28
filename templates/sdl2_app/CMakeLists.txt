cmake_minimum_required(VERSION 3.10)
project({{PROJECT_NAME}})

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} include)

add_executable({{PROJECT_NAME}}
    src/main.cpp
    src/app.cpp
)

target_link_libraries({{PROJECT_NAME}} ${SDL2_LIBRARIES})
