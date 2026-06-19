
# Create staticly linked library for the application.
add_library(Combat-Tracker-Lib STATIC
    App.h
    App.cpp
    Scene.h
    Scene.cpp
    Styling.h
    Window.h
    Window.cpp
    Utils.h
)
target_include_directories(Combat-Tracker-Lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link external dependencies to the static library.
target_link_libraries(Combat-Tracker-Lib PUBLIC
    glad
    glfw_target
    OpenGL::GL
    imgui
    glm::glm
    nlohmann_json::nlohmann_json
)