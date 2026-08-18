# Create staticly linked library for the application.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerLib/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/CombatTrackerLib/*.h)
add_library(Combat-Tracker-Lib STATIC ${SOURCES} ${HEADERS}  )

# src/ is the only include root, so every library header has exactly one spelling:
target_include_directories(Combat-Tracker-Lib PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# Link external dependencies to the static library.
target_link_libraries(Combat-Tracker-Lib
    PRIVATE
        Combat-Tracker-Options
    PUBLIC
        glad
        glfw_target
        OpenGL::GL
        imgui
        glm::glm
        stb_image
        nlohmann_json::nlohmann_json
)