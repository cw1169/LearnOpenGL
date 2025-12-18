cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW 
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Sources
set(SOURCES
    src/main.cpp
    src/glad.c
)

# Include directories
include_directories(
    src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLAD/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
)

# Executable
add_executable(opengl ${SOURCES})

# Link libraries
target_link_libraries(opengl
    OpenGL::GL
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
    dl
)

