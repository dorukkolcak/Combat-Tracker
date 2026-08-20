# Tests for the application half of the project.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/CombatTrackerApp/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/CombatTrackerApp/*.h)
add_executable(Combat-Tracker-App-Tests ${SOURCES} ${HEADERS})
target_include_directories(Combat-Tracker-App-Tests PRIVATE ${PROJECT_SOURCE_DIR})

# Notice we link the object library to the tests which doesn't include the main function of the application.
target_link_libraries(Combat-Tracker-App-Tests PRIVATE
    Combat-Tracker-App-Objects
    Combat-Tracker-Options
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(Combat-Tracker-App-Tests)