cmake_minimum_required(VERSION 3.10)
project(cppstarter)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Collect all source files
file(GLOB SRC_FILES "src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add specific flags for build types
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

# Install the built executable to the system's binary directory
# By default, this goes to /usr/local/bin or the path defined by CMAKE_INSTALL_PREFIX
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
