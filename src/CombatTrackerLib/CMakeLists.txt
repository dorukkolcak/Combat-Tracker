
# Create staticly linked library for the application.
add_library(Combat-Tracker-Lib STATIC
    App.h
    App.cpp
    Scene.h
    Scene.cpp
    Themes.h
    Window.h
    Window.cpp
    Utilities/UIUtils.h
    Shader.h 
    Shader.cpp 
    ShaderProgram.h 
    ShaderProgram.cpp
    Themes.cpp
    Texture2D.h
    Texture2D.cpp
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
    stb_image
)