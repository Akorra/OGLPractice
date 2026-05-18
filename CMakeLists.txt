cmake_minimum_required(VERSION 3.16)

project(OGLPractice VERSION 0.1.0 LANGUAGES C CXX)

# latest available c++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(oglpractice
    src/main.cpp
    src/shader.cpp
)

target_include_directories(oglpractice PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(oglpractice PRIVATE OpenGL::GL)

# FetchContent
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(oglpractice PRIVATE glfw)

# GLAD 2
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8 
)
FetchContent_MakeAvailable(glad2)

# Add the CMake folder from glad2 as a subdirectory
# GLAD2 includes its own CMake config for creating targets
add_subdirectory(
    ${glad2_SOURCE_DIR}/cmake
    ${glad2_BINARY_DIR}/cmake
)

# stb_image
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1  # Use a specific release tag
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# Create a GLAD loader target for OpenGL 
glad_add_library(
    glad_gl_core_46
    REPRODUCIBLE
    API gl:core=4.6
)

# Link GLM as a dependency
target_link_libraries(oglpractice PRIVATE glm::glm)
target_link_libraries(oglpractice PRIVATE glad_gl_core_46)
target_include_directories(oglpractice PRIVATE ${stb_SOURCE_DIR})