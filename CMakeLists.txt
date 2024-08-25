# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(lfp_a_utils C CXX)

# Set the paths for external dependencies
set(ArduinoCore_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ArduinoCore-avr)
set(ESP8266Core_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/ArduinoCore-esp8266)
set(GoogleTest_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/googletest)

# Ensure ArduinoCore is available
if(NOT EXISTS ${ArduinoCore_SOURCE_DIR})
  message(FATAL_ERROR "ArduinoCore not found. Please clone it into the external directory.")
endif()

# Ensure ESP8266Core is available
if(NOT EXISTS ${ESP8266Core_SOURCE_DIR})
  message(FATAL_ERROR "ESP8266Core not found. Please clone it into the external directory.")
endif()

# Ensure GoogleTest is available
if(NOT EXISTS ${GoogleTest_SOURCE_DIR})
  message(FATAL_ERROR "GoogleTest not found. Please clone it into the external directory.")
endif()

# Add options for target devices
option(TARGET_MICRO "Compile for Arduino Micro" OFF)
option(TARGET_UNO "Compile for Arduino UNO" OFF)
option(TARGET_ESP01 "Compile for ESP-01" OFF)
option(TARGET_NODEMCU "Compile for NodeMCU v2" OFF)

# Define target device macros and include directories
if(TARGET_MICRO)
  add_definitions(-DARDUINO_AVR_MICRO)
  include_directories(${ArduinoCore_SOURCE_DIR}/cores/arduino)
  include_directories(${ArduinoCore_SOURCE_DIR}/variants/standard)
  include_directories(/usr/lib/avr/include)
elseif(TARGET_UNO)
  add_definitions(-DARDUINO_AVR_UNO)
  include_directories(${ArduinoCore_SOURCE_DIR}/cores/arduino)
  include_directories(${ArduinoCore_SOURCE_DIR}/variants/standard)
  include_directories(/usr/lib/avr/include)
elseif(TARGET_ESP01)
  add_definitions(-DARDUINO_ESP8266_ESP01)
  include_directories(${ESP8266Core_SOURCE_DIR}/cores/esp8266)
  include_directories(${ESP8266Core_SOURCE_DIR}/variants/generic)
elseif(TARGET_NODEMCU)
  add_definitions(-DARDUINO_ESP8266_NODEMCU)
  add_definitions(-DESP8266)
  add_definitions(-DCORE_MOCK)
  include_directories(${ESP8266Core_SOURCE_DIR}/cores/esp8266)
  include_directories(${ESP8266Core_SOURCE_DIR}/variants/nodemcu)
  include_directories(${ESP8266Core_SOURCE_DIR}/tools/sdk/include)
  include_directories(${ESP8266Core_SOURCE_DIR}/include)
  include_directories(${CMAKE_SOURCE_DIR}/external/newlib-xtensa/newlib/libc/sys/xtensa/sys)
  include_directories(${CMAKE_SOURCE_DIR}/external/newlib-xtensa/newlib/libc/include/sys)
  include_directories(${CMAKE_SOURCE_DIR}/external/newlib-xtensa/newlib/libc/include)  # Include newlib headers
  include_directories(${CMAKE_SOURCE_DIR}/external/newlib-xtensa/newlib/libc/include/sys) 
else()
  message(FATAL_ERROR "No target device specified. Please set one of the TARGET_* options to ON.")
endif()

# Include directory for your project
include_directories(include)

# Gather all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add library target using the gathered source files
add_library(lfp_a_utils STATIC ${SOURCES})

# Set compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Add GoogleTest directories
add_subdirectory(${GoogleTest_SOURCE_DIR} ${CMAKE_BINARY_DIR}/googletest)

# Gather all test files
file(GLOB TEST_SOURCES "tests/*.cpp")

# Add test executable
add_executable(tests ${TEST_SOURCES})

# Create a static library target for ArduinoCore
file(GLOB ARDUINO_CORE_SOURCES
  ${ArduinoCore_SOURCE_DIR}/cores/arduino/*.c
  ${ArduinoCore_SOURCE_DIR}/cores/arduino/*.cpp
)
add_library(ArduinoCore STATIC ${ARDUINO_CORE_SOURCES})

# Create a static library target for ESP8266Core
file(GLOB ESP8266_CORE_SOURCES
  ${ESP8266Core_SOURCE_DIR}/cores/esp8266/*.c
  ${ESP8266Core_SOURCE_DIR}/cores/esp8266/*.cpp
)
add_library(ESP8266Core STATIC ${ESP8266_CORE_SOURCES})

# Link GoogleTest, the Arduino core, and your library to the test executable
target_link_libraries(tests gtest gtest_main lfp_a_utils ArduinoCore ESP8266Core)

# Enable testing
enable_testing()

# Add a test to be run by CTest
add_test(NAME logger_test COMMAND tests)