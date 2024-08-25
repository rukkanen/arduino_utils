# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(lfp_a_utils C CXX)

# Include FetchContent module
include(FetchContent)

# Fetch the ArduinoCore library
FetchContent_Declare(
  ArduinoCore
  GIT_REPOSITORY https://github.com/arduino/ArduinoCore-avr.git
  GIT_TAG master
)

# Make the content available
FetchContent_MakeAvailable(ArduinoCore)

# Include Arduino directories
include_directories(${ArduinoCore_SOURCE_DIR}/cores/arduino)
include_directories(${ArduinoCore_SOURCE_DIR}/variants/standard)

# Include directory for your project
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
file(GLOB TEST_SOURCES "tests/*.cpp")

# Add test executable
add_executable(tests ${TEST_SOURCES})

# Link GoogleTest, the Arduino core, and your library to the test executable
target_link_libraries(tests gtest gtest_main lfp_a_utils ${ArduinoCore_SOURCE_DIR}/cores/arduino)

# Enable testing
enable_testing()

# Add a test to be run by CTest
add_test(NAME logger_test COMMAND tests)
