cmake_minimum_required(VERSION 3.10)
project(librar_project)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build shared or static library (default: static)
option(BUILD_SHARED_LIBS "Build as a shared library" OFF)

# Include headers
include_directories(include)

# Collect all source files in src/
file(GLOB SRC_FILES "src/*.cpp")

# Exclude main.cpp from library sources
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Build library (static or shared depending on BUILD_SHARED_LIBS)
add_library(${PROJECT_NAME} ${SRC_FILES})

# Compile flags by build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building in DEBUG mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Building in RELEASE mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall)
else()
    message(STATUS "Building in unspecified mode (default compiler flags)")
endif()

# Test executable from src/main.cpp
add_executable(main_test src/main.cpp)
target_link_libraries(main_test PRIVATE ${PROJECT_NAME})
