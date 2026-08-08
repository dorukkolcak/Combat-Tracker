# Tests for the application half of the project.
set(APP_TEST_SOURCES

)
if(APP_TEST_SOURCES)
    add_executable(Combat-Tracker-App-Tests ${APP_TEST_SOURCES})
    target_link_libraries(Combat-Tracker-App-Tests PRIVATE
        Combat-Tracker-Lib
        Combat-Tracker-Options
        GTest::gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(Combat-Tracker-App-Tests)
endif()
