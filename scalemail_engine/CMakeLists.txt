cmake_minimum_required(VERSION 3.10)

project(scalemail_engine)

set(CMAKE_BUILD_TYPE Debug)

set(SCALEMAIL_ENGINE_INCLUDE
    include/blend.hpp
    include/camera.hpp
    include/ease.hpp
    include/fake_map.hpp
    include/font.hpp
    include/game_window.hpp
    include/gl_headers.hpp
    include/gl_util.hpp
    include/light.hpp
    include/math_util.hpp
    include/mesh.hpp
    include/render.hpp
    include/screen_capture.hpp
    include/shader.hpp
    include/sprite.hpp
    include/texture.hpp
    include/tiny_font.hpp
    include/transition.hpp)

set(SCALEMAIL_ENGINE_SOURCE
    src/blend.cpp
    src/camera.cpp
    src/fake_map.cpp
    src/font.cpp
    src/gl_util.cpp
    src/engine.cpp
    src/light.cpp
    src/mesh.cpp
    src/render.cpp
    src/screen_capture.cpp
    src/shader.cpp
    src/sprite.cpp
    src/texture.cpp
    src/transition.cpp
    src/tiny_font.cpp)

add_library(${PROJECT_NAME} ${SCALEMAIL_ENGINE_SOURCE} ${SCALEMAIL_ENGINE_SOURCE})
include_directories(include)
target_link_libraries(${PROJECT_NAME} glad glfw glm lodepng)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wextra")
    #target_compile_options(${PROJECT_NAME} PRIVATE "-Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/Wall")
endif()