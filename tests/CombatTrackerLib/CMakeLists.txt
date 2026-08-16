# Tests for the library half of the project.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/CombatTrackerLib/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/CombatTrackerLib/*.h)
add_executable(Combat-Tracker-Lib-Tests ${SOURCES} ${HEADERS})
target_include_directories(Combat-Tracker-Lib-Tests PRIVATE
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(Combat-Tracker-Lib-Tests PRIVATE
    Combat-Tracker-Lib
    Combat-Tracker-Options
    GTest::gtest_main
)

# Copy assets to the test build directory.
# Some test cases need to access assets as the test cases are actually mostly about testing the validity of assets.
# A POST_BUILD command only runs when the target itself relinks, which would leave the copy
# stale whenever an asset changes on its own. A custom target is always out of date, so the
# assets under test are refreshed on every build, not just on relink.
add_custom_target(Combat-Tracker-Lib-Tests-Assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Combat-Tracker-Lib-Tests>/assets
)
add_dependencies(Combat-Tracker-Lib-Tests Combat-Tracker-Lib-Tests-Assets)

include(GoogleTest)
gtest_discover_tests(Combat-Tracker-Lib-Tests)
