cmake_minimum_required(VERSION 3.10)

project(scalemail_engine)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SCALEMAIL_ENGINE_INCLUDE
    include/ai/blob_ai.hpp
    include/ai/skeleton_ai.hpp
    include/ai/vampire_ai.hpp
    include/gui/gui.hpp
    include/gui/gui_screen.hpp
    include/gui/pause_gui_screen.hpp
    include/gui/player_hud_gui_screen.hpp
    include/actor_util.hpp
    include/ai_behavior_factory.hpp
    include/ai_behavior.hpp
    include/ai_system.hpp
    include/ambient_light.hpp
    include/asset_manager.hpp
    include/blend.hpp
    include/bullet_data.hpp
    include/bullet_system.hpp
    include/bullet_util.hpp
    include/bury_system.hpp
    include/camera.hpp
    include/collision_group.hpp
    include/collision_test.hpp
    include/collision.hpp
    include/cursor.hpp
    include/damage_system.hpp
    include/direction_util.hpp
    include/direction.hpp
    include/door_system.hpp
    include/ease.hpp
    include/entity_collision.hpp
    include/entity_manager.hpp
    include/entity_system.hpp
    include/entity_types.hpp
    include/entity_util.hpp
    include/entity.hpp
    include/expire_system.hpp
    include/fade_shader.hpp
    include/font.hpp
    include/game_state_manager.hpp
    include/game_state.hpp
    include/game_window.hpp
    include/game.hpp
    include/gl_headers.hpp
    include/gl_util.hpp
    include/gun_system.hpp
    include/health_gauge.hpp
    include/health_system.hpp
    include/input_state.hpp
    include/input.hpp
    include/intro_game_state.hpp
    include/inventory_system.hpp
    include/layer.hpp
    include/light_data.hpp
    include/light_system.hpp
    include/light.hpp
    include/line_shader.hpp
    include/loot_system.hpp
    include/main_game_state.hpp
    include/map_load.hpp
    include/map_mesh.hpp
    include/map_render.hpp
    include/map.hpp
    include/math_util.hpp
    include/mesh.hpp
    include/name_system.hpp
    include/particle_system.hpp
    include/physics_system.hpp
    include/player_util.hpp
    include/player.hpp
    include/prefab_factory.hpp
    include/quad_shader.hpp
    include/random.hpp
    include/rectangle.hpp
    include/render.hpp
    include/screen_capture.hpp
    include/shader.hpp
    include/sprite_animation.hpp
    include/sprite_batch.hpp
    include/sprite_effect_system.hpp
    include/sprite_shader.hpp
    include/sprite_system.hpp
    include/sprite.hpp
    include/static_collision.hpp
    include/string_util.hpp
    include/texture.hpp
    include/tile_shader.hpp
    include/tileset.hpp
    include/tiny_font.hpp
    include/transition.hpp
    include/trigger_collision.hpp
    include/trigger_system.hpp
    include/trigger.hpp
    include/vector_util.hpp
    include/vertex_data.hpp
    include/world.hpp)

set(SCALEMAIL_ENGINE_SOURCE
    src/ai/blob_ai.cpp
    src/ai/skeleton_ai.cpp
    src/ai/vampire_ai.cpp
    src/entity_types/bullet.cpp
    src/entity_types/door.cpp
    src/entity_types/loot.cpp
    src/gui/gui.cpp
    src/gui/gui_screen.cpp
    src/gui/pause_gui_screen.cpp
    src/gui/player_hud_gui_screen.cpp
    src/ai_behavior_factory.cpp
    src/ai_system.cpp
    src/actor_util.cpp
    src/ambient_light.cpp
    src/asset_manager.cpp
    src/blend.cpp
    src/bullet_system.cpp
    src/bullet_util.cpp
    src/bury_system.cpp
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
    src/entity_util.cpp
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
    src/inventory_system.cpp
    src/layer.cpp
    src/light.cpp
    src/light_system.cpp
    src/loot_system.cpp
    src/main_game_state.cpp
    src/mesh.cpp
    src/map.cpp
    src/map_load.cpp
    src/map_mesh.cpp
    src/map_render.cpp
    src/name_system.cpp
    src/particle_system.cpp
    src/physics_system.cpp
    src/player_util.cpp
    src/prefab_factory.cpp
    src/render.cpp
    src/screen_capture.cpp
    src/shader.cpp
    src/sprite.cpp
    src/sprite_batch.cpp
    src/sprite_effect_system.cpp
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

target_compile_options(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:DEBUG>:-g3>"
    "$<$<CONFIG:RELEASE>:-O3>")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wextra")
    #target_compile_options(${PROJECT_NAME} PRIVATE "-Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/Wall")
endif()