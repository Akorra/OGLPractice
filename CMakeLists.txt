cmake_minimum_required(VERSION 3.16)

project(OGLPractice VERSION 0.1.0 LANGUAGES C CXX)

# latest available c++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Load all dependencies once
include(cmake/deps.cmake)

# Shared library
add_subdirectory(common)

# Add SubChapter Exercise
add_subdirectory(chapters/AdvancedOpenGL/3_Blending)
add_subdirectory(chapters/AdvancedOpenGL/4_FaceCulling)


