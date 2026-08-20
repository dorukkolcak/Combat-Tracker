# Create object library for the application.
# We don't create an executable blindly just yet as the tests need to link against this object library.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/*.h)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/main.cpp)

add_library(Combat-Tracker-App-Objects OBJECT ${SOURCES} ${HEADERS})

# Link the static library we created earlier to the application.
target_link_libraries(
    Combat-Tracker-App-Objects
    PRIVATE
        Combat-Tracker-Options
    PUBLIC
        Combat-Tracker-Lib
)

# Add the main file and link against our object and options libraries to create the final executable.
add_executable(Combat-Tracker-App ${PROJECT_SOURCE_DIR}/src/CombatTrackerApp/main.cpp)
target_link_libraries(
    Combat-Tracker-App PRIVATE
    Combat-Tracker-App-Objects
    Combat-Tracker-Options
)

# Copy assets to the build directory after creating the executable.
add_custom_command(TARGET Combat-Tracker-App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Combat-Tracker-App>/assets
)
