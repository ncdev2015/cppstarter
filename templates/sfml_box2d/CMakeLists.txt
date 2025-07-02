cmake_minimum_required(VERSION 3.10)
project(sfml_box2d_app)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# Find SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Find Box2D
find_package(box2d REQUIRED)
# Alternative if the above doesn't work:
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(BOX2D REQUIRED box2d)

# Include headers
include_directories(include)

# Source files
file(GLOB SRC_FILES "src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Link Box2D library
target_link_libraries(${PROJECT_NAME} box2d::box2d)
# Alternative if the above doesn't work:
# target_link_libraries(${PROJECT_NAME} ${BOX2D_LIBRARIES})
# target_include_directories(${PROJECT_NAME} PRIVATE ${BOX2D_INCLUDE_DIRS})
# target_compile_options(${PROJECT_NAME} PRIVATE ${BOX2D_CFLAGS_OTHER})

# Compiler flags per build type
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