cmake_minimum_required(VERSION 3.10)
project(sdl2_app)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)

# Include headers
include_directories(${SDL2_INCLUDE_DIRS} include)

# Source files
file(GLOB SRC_FILES "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Build type-specific compile options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Compiling in DEBUG mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Compiling in RELEASE mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall)
else()
    message(STATUS "Compiling in unspecified mode: ${CMAKE_BUILD_TYPE}")
endif()
