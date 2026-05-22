include(FetchContent)

# GLFW
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest)
FetchContent_MakeAvailable(glfw)

# GLAD 2
FetchContent_Declare(glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8)
FetchContent_MakeAvailable(glad2)
add_subdirectory(${glad2_SOURCE_DIR}/cmake ${glad2_BINARY_DIR}/cmake)
glad_add_library(glad_gl_core_46 REPRODUCIBLE API gl:core=4.6)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1  # Use a specific release tag
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master)
FetchContent_MakeAvailable(stb)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL     OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3)
FetchContent_MakeAvailable(assimp)