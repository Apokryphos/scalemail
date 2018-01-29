#include "camera.hpp"
#include "ease.hpp"
#include "font.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "main_game_state.hpp"
#include "light_system.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

namespace ScaleMail
{
const float cameraStartPosition = -1024.0f;
static const glm::vec4 ambientColor(0.3f, 0.38f, 0.4f, 1.0f);

//	============================================================================
MainGameState::MainGameState() {
}

//	============================================================================
void MainGameState::draw(const GameWindow& gameWindow, Camera& camera) {
}

//	============================================================================
void MainGameState::initialize(World& world, Camera& camera) {
    world.getLightSystem().setAmbientColor(ambientColor);
    camera.position = glm::vec2(0, cameraStartPosition);
}

//	============================================================================
void MainGameState::update(World& world, Camera& camera, float elapsedSeconds) {
}
}
