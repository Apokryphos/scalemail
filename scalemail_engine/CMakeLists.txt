cmake_minimum_required (VERSION 2.9)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Werror")

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

project(scalemail_engine)

set(SCALEMAIL_ENGINE_INCLUDE
    include/blend.hpp
    include/camera.hpp
    include/ease.hpp
    include/fake_map.hpp
    include/font.hpp
    include/gl_headers.hpp
    include/gl_util.hpp
    include/light.hpp
    include/mesh.hpp
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
    src/screen_capture.cpp
    src/shader.cpp
    src/sprite.cpp
    src/texture.cpp
    src/transition.cpp
    src/tiny_font.cpp)

add_library(${PROJECT_NAME} ${SCALEMAIL_ENGINE_SOURCE} ${SCALEMAIL_ENGINE_SOURCE})
include_directories(include)
target_link_libraries(${PROJECT_NAME} glad glfw glm lodepng)