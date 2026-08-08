cmake_minimum_required(VERSION 3.24)
project(Combat-Tracker LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies.cmake)

# CMAKE_COMPILE_WARNING_AS_ERROR is deliberately set here rather than at the top of the file.
# The COMPILE_WARNING_AS_ERROR property of a target is initialized from this variable at the moment the target is created,
# so setting it after the dependencies are declared leaves every fetched and vendored target out of it and covers only ours.
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# One place to keep the language standard and the warning flags, linked privately into each of our own targets.
# An INTERFACE library is used because it carries no sources or build output of its own, it only hands its usage
# requirements to whatever links it, which keeps the four targets from repeating the same generator expressions.
add_library(Combat-Tracker-Options INTERFACE)
target_compile_features(Combat-Tracker-Options INTERFACE cxx_std_23)
target_compile_options(Combat-Tracker-Options INTERFACE
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W4;/utf-8>
    $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wall;-Wextra>
)

# Setup directories.
add_subdirectory(src/CombatTrackerLib)
add_subdirectory(src/CombatTrackerApp)
if(BUILD_TESTING)
    add_subdirectory(tests/CombatTrackerLib)
    add_subdirectory(tests/CombatTrackerApp)
endif()
