cmake_minimum_required(VERSION 3.25)
project(Combat-Tracker LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies.cmake)

# CMAKE_COMPILE_WARNING_AS_ERROR is deliberately set here rather than at the top of the file.
# The COMPILE_WARNING_AS_ERROR property of a target is initialized from this variable at the moment the target is created,
# so setting it after the dependencies are declared leaves every fetched and vendored target out of it and covers only ours.
# We only set this during the CI builds as it makes testing things locally really cumbersome.
if(DEFINED ENV{CI})
    set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif()

# This conditional tries to mimic the "treat all informational warnings as errors" behaviour across all compilers.
# I don't think it is possible to have a direct reflection of all warnings for all compilers. 
# This is where Github Workflows should hopefully catch some erros.
# Feel free to play around with the options here.
if(MSVC)
    set(CXX_WARNINGS 
        /W4          # Baseline strict warnings
        /utf-8       # Enforce UTF-8 source/execution character sets
        /w15038      # Warn about out-of-order initializer lists
        /w14265      # Warn about classes with virtual functions but non-virtual destructors
        /w14263      # Member function hides base class virtual function without overriding it
        /w14266      # No override available for virtual function overload; function is hidden
        /w14242      # Warn about conversions with possible data loss
        /w14287      # Warn about unsigned/negative constant mismatches
        /permissive- # Enforce strict standards-conforming C++ conformance
    )
else()
    set(CXX_WARNINGS 
        -Wall 
        -Wextra 
        -Wpedantic
        -Wshadow            # Local variables shadowing outer scopes
        -Wnon-virtual-dtor  # Missing virtual destructors (match /w14265)
        -Woverloaded-virtual# Overloaded virtual functions hide base variants (match /w14263 & /w14266)
        -Wcast-align        # Warn about pointer casts that increase alignment requirements
        -Wold-style-cast    # Force static_cast / reinterpret_cast over C-casts
        -Wconversion        # Explicitly flag implicit type truncation/coercion
        -Wunused            # Catch forgotten dead code/variables
    )
endif()


# One place to keep the language standard and the warning flags, linked privately into each of our own targets.
# An INTERFACE library is used because it carries no sources or build output of its own, it only hands its usage
# requirements to whatever links it, which keeps the four targets from repeating the same generator expressions.
add_library(Combat-Tracker-Options INTERFACE)
target_compile_features(Combat-Tracker-Options INTERFACE cxx_std_23)
target_compile_options(Combat-Tracker-Options INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:${CXX_WARNINGS}>
)

# This forces us to not include third-party dependencies using double braces. We should include them using angle brackets.
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Combat-Tracker-Options INTERFACE /external:W0 /external:anglebrackets)
endif()


# Sanitizer options added to Combat-Tracker-Options.
# Keep in mind that Asan and UBsan requires GCC and Clang.
option(COMBAT_TRACKER_SANITIZE "Build our own targets with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(COMBAT_TRACKER_SANITIZE)
    if(WIN32)
        message(FATAL_ERROR "COMBAT_TRACKER_SANITIZE requires GCC or Clang on Linux or macOS.")
    endif()

    set(SANITIZER_FLAGS
        -fsanitize=address,undefined
        -fno-sanitize-recover=all
        -fno-omit-frame-pointer
    )
    target_compile_options(Combat-Tracker-Options INTERFACE ${SANITIZER_FLAGS})
    target_link_options(Combat-Tracker-Options INTERFACE ${SANITIZER_FLAGS})
endif()

# Setup directories.
add_subdirectory(src/CombatTrackerLib)
add_subdirectory(src/CombatTrackerApp)
if(BUILD_TESTING)
    add_subdirectory(tests/CombatTrackerLib)
    add_subdirectory(tests/CombatTrackerApp)
endif()
