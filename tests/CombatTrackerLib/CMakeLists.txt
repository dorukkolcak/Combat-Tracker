# Tests for the library half of the project.
set(LIB_TEST_SOURCES

)
if(LIB_TEST_SOURCES)
    add_executable(Combat-Tracker-Lib-Tests ${LIB_TEST_SOURCES})
    target_link_libraries(Combat-Tracker-Lib-Tests PRIVATE
        Combat-Tracker-Lib
        Combat-Tracker-Options
        GTest::gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(Combat-Tracker-Lib-Tests)
endif()
