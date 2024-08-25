# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(lfp_a_utils C CXX)

# Include directory
include_directories(include)

# Gather all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add library target using the gathered source files
add_library(lfp_a_utils STATIC ${SOURCES})

# Set compiler flags
target_compile_options(lfp_a_utils PRIVATE -Wall -Wextra -Werror -std=c11)

# Add GoogleTest
add_subdirectory(googletest)

# Gather all test files
file(GLOB TEST_SOURCES "test/*.cpp")

# Add test executable
add_executable(tests ${TEST_SOURCES})

# Link GoogleTest and the library to the test executable
target_link_libraries(tests gtest gtest_main lfp_a_utils)

# Enable testing
enable_testing()

# Add a test to be run by CTest
add_test(NAME logger_test COMMAND tests)
