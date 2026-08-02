# Tests for the application half of the project.

target_include_directories(Combat-Tracker-App-Tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp
)

target_link_libraries(Combat-Tracker-App-Tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Combat-Tracker-App-Tests)
