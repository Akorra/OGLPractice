add_library(ogl_common STATIC
    shader.cpp
    model.cpp
)

target_include_directories(ogl_common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${stb_SOURCE_DIR}
)

find_package(OpenGL REQUIRED)
target_link_libraries(ogl_common PUBLIC
    OpenGL::GL
    glfw
    glad_gl_core_46
    glm::glm
    assimp
)