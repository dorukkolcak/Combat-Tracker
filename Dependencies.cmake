# Download releases as dependecies at configure time.
include(FetchContent)

# GLFW
find_package(glfw3 3.4 QUIET)

if (glfw3_FOUND)
    if (TARGET glfw3::glfw)
        add_library(glfw_target ALIAS glfw3::glfw)
    elseif (TARGET glfw)
        add_library(glfw_target ALIAS glfw)
    else()
        message(FATAL_ERROR "glfw3 found, but no known CMake target was provided.")
    endif()
else()
    FetchContent_Declare(
        glfw3
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    )

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw3)

    if (TARGET glfw)
        add_library(glfw_target ALIAS glfw)
    elseif (TARGET glfw3::glfw)
        add_library(glfw_target ALIAS glfw3::glfw)
    else()
        message(FATAL_ERROR "GLFW was fetched, but no known CMake target was provided.")
    endif()
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_library(glad STATIC
    ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c)

target_include_directories(glad PUBLIC
    ${PROJECT_SOURCE_DIR}/external/glad/include
)

# glad's loader resolves symbols through dlopen/dlsym on every non-Windows
# platform, which lives in libdl on glibc older than 2.34.
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

#set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# GLM
find_package(glm 1.0.1 QUIET)

if (NOT glm_FOUND)
    FetchContent_Declare(
        glm
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    )

    FetchContent_MakeAvailable(glm)
endif()

if (NOT TARGET glm::glm)
    message(FATAL_ERROR "GLM target glm::glm was not found.")
endif()

# ImGui
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/external/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMGUI_DIR}/misc/cpp
)

# stb_image.h
add_library(stb_image STATIC
    ${PROJECT_SOURCE_DIR}/external/stb/stb_image.cpp
    ${PROJECT_SOURCE_DIR}/external/stb/stb_image.h
)
target_include_directories(
    stb_image PUBLIC
    ${PROJECT_SOURCE_DIR}/external/stb
)

# Link all external libraries
target_link_libraries(imgui PUBLIC
    glfw_target
    OpenGL::GL
    glad
)
target_compile_definitions(imgui PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

set_target_properties(imgui PROPERTIES FOLDER "Dependencies")

# nlohmann/json
FetchContent_Declare(
    json
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

if (NOT TARGET nlohmann_json::nlohmann_json)
    message(FATAL_ERROR "nlohmann_json::nlohmann_json target was not found.")
endif()

# Google Tests
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)