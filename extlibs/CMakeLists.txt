cmake_minimum_required (VERSION 3.10)

# Earcut
add_library(earcut_hpp ${CMAKE_CURRENT_SOURCE_DIR}/earcut.hpp/include/mapbox/earcut.hpp)
set_target_properties(earcut_hpp PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(earcut_hpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/earcut.hpp/include)

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

# TmxMapLib
add_subdirectory(TmxMapLib)