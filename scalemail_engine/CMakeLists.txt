cmake_minimum_required(VERSION 3.10)

project(scalemail_engine)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SCALEMAIL_ENGINE_SOURCE
    src/ai/ai_behavior_factory.cpp
    src/ai/ai_node.cpp
    src/ai/ai_tree.cpp
    src/ai/ai_whiteboard.cpp
    src/ai/ai_behaviors/ally_ai.cpp
    src/ai/ai_behaviors/bat_ai.cpp
    src/ai/ai_behaviors/blob_ai.cpp
    src/ai/ai_behaviors/skeleton_ai.cpp
    src/ai/ai_behaviors/skeleton_warrior_ai.cpp
    src/ai/ai_behaviors/vampire_ai.cpp
    src/ai/ai_behaviors/wisp_ai.cpp
    src/ai/ai_nodes/cooldown_ai_node.cpp
    src/ai/ai_nodes/composite_ai_node.cpp
    src/ai/ai_nodes/decorator_ai_node.cpp
    src/ai/ai_nodes/entity_count_ai_node.cpp
    src/ai/ai_nodes/failure_ai_node.cpp
    src/ai/ai_nodes/fire_at_target_ai_node.cpp
    src/ai/ai_nodes/function_ai_node.cpp
    src/ai/ai_nodes/inverter_ai_node.cpp
    src/ai/ai_nodes/random_move_direction_ai_node.cpp
    src/ai/ai_nodes/seek_target_ai_node.cpp
    src/ai/ai_nodes/selector_ai_node.cpp
    src/ai/ai_nodes/sequence_ai_node.cpp
    src/ai/ai_nodes/success_ai_node.cpp
    src/ai/ai_nodes/target_attacker_ai_node.cpp
    src/ai/ai_nodes/target_range_ai_node.cpp
    src/ai/ai_nodes/wander_ai_node.cpp
    src/entity_types/actor.cpp
    src/entity_types/bullet.cpp
    src/entity_types/door.cpp
    src/entity_types/loot.cpp
    src/entity_types/prop.cpp
    src/entity_types/torch.cpp
    src/entity_types/trigger.cpp
    src/gui/gui.cpp
    src/gui/gui_screen.cpp
    src/gui/pause_gui_screen.cpp
    src/gui/player_hud_gui_screen.cpp
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
    src/facing_system.cpp
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
    src/map_model.cpp
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
    src/team_system.cpp
    src/texture.cpp
    src/tileset.cpp
    src/transition.cpp
    src/trigger_system.cpp
    src/tiny_font.cpp
    src/vertex_attrib.cpp
    src/vertex_data.cpp
    src/world.cpp)

add_library(${PROJECT_NAME} ${SCALEMAIL_ENGINE_SOURCE} ${SCALEMAIL_ENGINE_SOURCE})
include_directories(include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
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