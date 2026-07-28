cmake_minimum_required(VERSION 3.24)
project(Combat-Tracker LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies.cmake)

enable_testing()

add_subdirectory(src/CombatTrackerLib)
add_subdirectory(src/CombatTrackerApp)
add_subdirectory(tests/CombatTrackerLib)
add_subdirectory(tests/CombatTrackerApp)

# Suppress character conversion warning in Google Test for macOS with C++23
# This was captured via Github Workflows
if(APPLE AND CMAKE_CXX_STANDARD GREATER_EQUAL 23)
  target_compile_options(gtest PRIVATE -Wno-character-conversion)
  target_compile_options(gtest_main PRIVATE -Wno-character-conversion)
endif()