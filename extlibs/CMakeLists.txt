cmake_minimum_required (VERSION 2.9)

# GLAD
add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLM
add_subdirectory(glm)

# LodePNG
add_library(lodepng
    ${CMAKE_CURRENT_SOURCE_DIR}/lodepng/lodepng.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lodepng/lodepng.h)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lodepng)