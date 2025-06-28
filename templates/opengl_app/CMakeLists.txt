cmake_minimum_required(VERSION 3.10)
project({{PROJECT_NAME}})

set(CMAKE_CXX_STANDARD 17)

# Include paths
include_directories(include)

# Add source files
add_executable({{PROJECT_NAME}}
    src/main.cpp
    src/shader_utils.cpp
)

# Find GLFW
find_package(glfw3 REQUIRED)

# Link GLAD manually (or via vcpkg, submodule, etc.)
add_subdirectory(external/glad)

target_link_libraries({{PROJECT_NAME}} glfw glad ${CMAKE_DL_LIBS})
