# Tests for the library half of the project.

target_link_libraries(Combat-Tracker-Lib-Tests PRIVATE
    Combat-Tracker-Lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Combat-Tracker-Lib-Tests)
