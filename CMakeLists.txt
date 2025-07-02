cmake_minimum_required(VERSION 3.16)
project(cppstarter)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(include)

# Collect all source files (excluding main.cpp for library)
file(GLOB SRC_FILES "src/*.cpp")

# Remove main.cpp from library sources if it exists
list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.cpp$")

# Create a library with the source files (excluding main)
if(SRC_FILES)
    add_library(${PROJECT_NAME}_lib ${SRC_FILES})
    target_include_directories(${PROJECT_NAME}_lib PUBLIC include)
endif()

# Define the main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the library to the executable if it exists
if(SRC_FILES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
endif()

# Add specific flags for build types
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building in DEBUG mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)
    if(SRC_FILES)
        target_compile_definitions(${PROJECT_NAME}_lib PRIVATE DEBUG)
        target_compile_options(${PROJECT_NAME}_lib PRIVATE -g -O0 -Wall -Wextra)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Building in RELEASE mode")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra)
    if(SRC_FILES)
        target_compile_definitions(${PROJECT_NAME}_lib PRIVATE NDEBUG)
        target_compile_options(${PROJECT_NAME}_lib PRIVATE -O3 -Wall -Wextra)
    endif()
else()
    message(STATUS "Building in unspecified mode (default compiler flags)")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Install the built executable to the system's binary directory
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# -------- GoogleTest integration --------
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    # Set policy for FetchContent timestamps
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()

    include(FetchContent)
    
    # Disable installation of googletest
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
        GIT_SHALLOW    TRUE
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # Make googletest available
    FetchContent_MakeAvailable(googletest)
    
    # Enable testing
    enable_testing()
    
    # Find test files
    file(GLOB TEST_FILES "test/*.cpp" "tests/*.cpp")
    
    if(TEST_FILES)
        # Create test executable
        add_executable(${PROJECT_NAME}_tests ${TEST_FILES})
        
        # Link with Google Test and your library
        target_link_libraries(${PROJECT_NAME}_tests 
            PRIVATE 
            gtest 
            gtest_main
        )
        
        # Link with project library if it exists
        if(SRC_FILES)
            target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}_lib)
        endif()
        
        # Include directories for tests
        target_include_directories(${PROJECT_NAME}_tests PRIVATE include)
        
        # Include GoogleTest module for test discovery
        include(GoogleTest)
        gtest_discover_tests(${PROJECT_NAME}_tests)
        
        message(STATUS "Tests enabled - found ${TEST_FILES}")
    else()
        message(WARNING "BUILD_TESTS is ON but no test files found in test/ or tests/ directories")
    endif()
else()
    message(STATUS "Tests disabled")
endif()

# -------- Valgrind integration --------
find_program(VALGRIND_EXECUTABLE valgrind)

if(VALGRIND_EXECUTABLE)
    message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}")
    
    # Common Valgrind options
    set(VALGRIND_COMMON_OPTIONS
        --tool=memcheck
        --leak-check=full
        --show-leak-kinds=all
        --track-origins=yes
        --verbose
        --error-exitcode=1
    )
    
    # Valgrind options for detailed analysis
    set(VALGRIND_DETAILED_OPTIONS
        ${VALGRIND_COMMON_OPTIONS}
        --track-fds=yes
        --show-reachable=yes
        --keep-stacktraces=alloc-and-free
        --freelist-vol=10000000
        --freelist-big-blocks=10000000
    )
    
    # Valgrind suppressions file (optional)
    set(VALGRIND_SUPPRESSIONS_FILE "${CMAKE_SOURCE_DIR}/valgrind.supp")
    if(EXISTS ${VALGRIND_SUPPRESSIONS_FILE})
        list(APPEND VALGRIND_COMMON_OPTIONS --suppressions=${VALGRIND_SUPPRESSIONS_FILE})
        list(APPEND VALGRIND_DETAILED_OPTIONS --suppressions=${VALGRIND_SUPPRESSIONS_FILE})
        message(STATUS "Using Valgrind suppressions: ${VALGRIND_SUPPRESSIONS_FILE}")
    endif()
    
else()
    message(STATUS "Valgrind not found - memory checking targets will not be available")
endif()

# Custom targets for convenience
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running ${PROJECT_NAME}"
)

# Valgrind targets for main executable
if(VALGRIND_EXECUTABLE)
    add_custom_target(valgrind
        COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_COMMON_OPTIONS} ./${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} with Valgrind (memory check)"
    )
    
    add_custom_target(valgrind-detailed
        COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_DETAILED_OPTIONS} ./${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} with Valgrind (detailed analysis)"
    )
    
    # Helgrind for thread checking
    add_custom_target(helgrind
        COMMAND ${VALGRIND_EXECUTABLE} --tool=helgrind --verbose --error-exitcode=1 ./${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} with Helgrind (thread error detection)"
    )
    
    # Cachegrind for performance analysis
    add_custom_target(cachegrind
        COMMAND ${VALGRIND_EXECUTABLE} --tool=cachegrind --verbose ./${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} with Cachegrind (cache profiling)"
    )
endif()

if(BUILD_TESTS AND TEST_FILES)
    add_custom_target(test_run
        COMMAND ${PROJECT_NAME}_tests
        DEPENDS ${PROJECT_NAME}_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests"
    )
    
    # Valgrind targets for tests
    if(VALGRIND_EXECUTABLE)
        add_custom_target(test_valgrind
            COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_COMMON_OPTIONS} ./${PROJECT_NAME}_tests
            DEPENDS ${PROJECT_NAME}_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests with Valgrind (memory check)"
        )
        
        add_custom_target(test_valgrind-detailed
            COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_DETAILED_OPTIONS} ./${PROJECT_NAME}_tests
            DEPENDS ${PROJECT_NAME}_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests with Valgrind (detailed analysis)"
        )
        
        # Helgrind for tests
        add_custom_target(test_helgrind
            COMMAND ${VALGRIND_EXECUTABLE} --tool=helgrind --verbose --error-exitcode=1 ./${PROJECT_NAME}_tests
            DEPENDS ${PROJECT_NAME}_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests with Helgrind (thread error detection)"
        )
    endif()
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "  Valgrind Available: ${VALGRIND_EXECUTABLE}")
if(SRC_FILES)
    list(LENGTH SRC_FILES SRC_COUNT)
    message(STATUS "  Source Files: ${SRC_COUNT}")
endif()
if(VALGRIND_EXECUTABLE)
    message(STATUS "")
    message(STATUS "Available Valgrind targets:")
    message(STATUS "  make valgrind          - Memory check (basic)")
    message(STATUS "  make valgrind-detailed - Memory check (detailed)")
    message(STATUS "  make helgrind          - Thread error detection")
    message(STATUS "  make cachegrind        - Cache profiling")
    if(BUILD_TESTS AND TEST_FILES)
        message(STATUS "  make test_valgrind     - Run tests with Valgrind")
        message(STATUS "  make test_helgrind     - Run tests with Helgrind")
    endif()
endif()
message(STATUS "")