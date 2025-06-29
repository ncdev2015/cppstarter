cmake_minimum_required(VERSION 3.10)
project(opengl_app)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include header directory
include_directories(include)

# Enable build type options if not set (optional)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build (Debug or Release)" FORCE)
endif()

# Find required libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Source files
file(GLOB SRC_FILES "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)

# Compilation flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Compiling in DEBUG mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Compiling in RELEASE mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall)
else()
    message(STATUS "Compiling in mode: ${CMAKE_BUILD_TYPE}")
endif()
