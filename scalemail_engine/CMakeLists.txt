cmake_minimum_required(VERSION 3.10)

project(scalemail_engine)

set(CMAKE_BUILD_TYPE Debug)

set(SCALEMAIL_ENGINE_INCLUDE
    include/ai/vampire_ai.hpp
    include/ai_behavior.hpp
    include/ai_behavior_factory.hpp
    include/ai_system.hpp
    include/ambient_light.hpp
    include/asset_manager.hpp
    include/blend.hpp
    include/bullet_system.hpp
    include/bullet_util.hpp
    include/camera.hpp
    include/collision.hpp
    include/collision_group.hpp
    include/collision_test.hpp
    include/cursor.hpp
    include/damage_system.hpp
    include/direction.hpp
    include/direction_util.hpp
    include/door_system.hpp
    include/ease.hpp
    include/entity.hpp
    include/entity_manager.hpp
    include/entity_system.hpp
    include/expire_system.hpp
    include/fade_shader.hpp
    include/font.hpp
    include/game.hpp
    include/game_state.hpp
    include/game_state_manager.hpp
    include/game_window.hpp
    include/gl_headers.hpp
    include/gl_util.hpp
    include/gun_system.hpp
    include/health_gauge.hpp
    include/health_system.hpp
    include/input.hpp
    include/input_state.hpp
    include/intro_game_state.hpp
    include/layer.hpp
    include/light.hpp
    include/light_system.hpp
    include/main_game_state.hpp
    include/map.hpp
    include/map_load.hpp
    include/map_render.hpp
    include/math_util.hpp
    include/mesh.hpp
    include/name_system.hpp
    include/physics_system.hpp
    include/player.hpp
    include/player_util.hpp
    include/prefab_factory.hpp
    include/quad_shader.hpp
    include/random.hpp
    include/render.hpp
    include/screen_capture.hpp
    include/shader.hpp
    include/sprite.hpp
    include/sprite_batch.hpp
    include/sprite_shader.hpp
    include/sprite_system.hpp
    include/string_util.hpp
    include/texture.hpp
    include/tileset.hpp
    include/tile_shader.hpp
    include/tiny_font.hpp
    include/transition.hpp
    include/trigger_system.hpp
    include/vertex_data.hpp
    include/world.hpp)

set(SCALEMAIL_ENGINE_SOURCE
    src/ai/vampire_ai.cpp
    src/ai_behavior_factory.cpp
    src/ai_system.cpp
    src/ambient_light.cpp
    src/asset_manager.cpp
    src/blend.cpp
    src/bullet_system.cpp
    src/bullet_util.cpp
    src/camera.cpp
    src/collision.cpp
    src/collision_test.cpp
    src/cursor.cpp
    src/damage_system.cpp
    src/direction_util.cpp
    src/door_system.cpp
    src/entity.cpp
    src/entity_manager.cpp
    src/entity_system.cpp
    src/expire_system.cpp
    src/font.cpp
    src/gl_util.cpp
    src/engine.cpp
    src/game_state.cpp
    src/game_state_manager.cpp
    src/gun_system.cpp
    src/health_gauge.cpp
    src/health_system.cpp
    src/input.cpp
    src/intro_game_state.cpp
    src/layer.cpp
    src/light.cpp
    src/light_system.cpp
    src/main_game_state.cpp
    src/mesh.cpp
    src/map.cpp
    src/map_load.cpp
    src/map_render.cpp
    src/name_system.cpp
    src/physics_system.cpp
    src/player_util.cpp
    src/prefab_factory.cpp
    src/render.cpp
    src/screen_capture.cpp
    src/shader.cpp
    src/sprite.cpp
    src/sprite_batch.cpp
    src/sprite_system.cpp
    src/string_util.cpp
    src/texture.cpp
    src/tileset.cpp
    src/transition.cpp
    src/trigger_system.cpp
    src/tiny_font.cpp
    src/vertex_data.cpp
    src/world.cpp)

add_library(${PROJECT_NAME} ${SCALEMAIL_ENGINE_SOURCE} ${SCALEMAIL_ENGINE_SOURCE})
include_directories(include)
target_link_libraries(${PROJECT_NAME} glad glfw glm lodepng TmxMapLib)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wextra")
    #target_compile_options(${PROJECT_NAME} PRIVATE "-Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/Wall")
endif()