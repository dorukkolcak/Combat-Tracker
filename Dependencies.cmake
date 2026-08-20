find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
endif()

add_library(glad STATIC
    ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c
)
add_library(stb_image STATIC
    ${PROJECT_SOURCE_DIR}/external/stb/stb_image.cpp
    ${PROJECT_SOURCE_DIR}/external/stb/stb_image.h
)
target_include_directories(
    glad SYSTEM PUBLIC
    ${PROJECT_SOURCE_DIR}/external/glad/include
    stb_image SYSTEM PUBLIC
    ${PROJECT_SOURCE_DIR}/external/stb
)
target_link_libraries(glad PUBLIC
    ${CMAKE_DL_LIBS}
)
