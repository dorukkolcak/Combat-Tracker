# Create executable for the application.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/*.h)
add_executable(Combat-Tracker-App ${SOURCES} ${HEADERS})

# Link the static library we created earlier to the application.
target_link_libraries(
    Combat-Tracker-App PRIVATE
    Combat-Tracker-Lib
    Combat-Tracker-Options
)