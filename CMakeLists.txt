cmake_minimum_required(VERSION 3.14)
project(Combat-Tracker LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies.cmake)

add_subdirectory(src/CombatTrackerLib)
add_subdirectory(src/CombatTrackerApp)