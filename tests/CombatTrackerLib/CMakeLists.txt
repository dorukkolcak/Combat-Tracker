# Tests for the library half of the project.

add_executable(Combat-Tracker-Lib-Tests
    IOUtilsTests.cpp
    ThemesTests.cpp
)

target_link_libraries(Combat-Tracker-Lib-Tests PRIVATE
    Combat-Tracker-Lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Combat-Tracker-Lib-Tests)
