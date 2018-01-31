#include "camera.hpp"
#include "ease.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "main_game_state.hpp"
#include "light_system.hpp"
#include "physics_system.hpp"
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
MainGameState::MainGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager) {
}

//	============================================================================
void MainGameState::activate(Game& game) {
	World& world = *game.world;
	world.getLightSystem().setAmbientColor(ambientColor);

	Camera& camera = *game.camera;
	camera.position = glm::vec2(0, cameraStartPosition);
}

//	============================================================================
void MainGameState::draw(const Game& game, Camera& camera) {
}

//	============================================================================
void MainGameState::initialize(Game& game) {
}

//	============================================================================
void MainGameState::update(World& world, Camera& camera, float elapsedSeconds) {
	std::vector<Player*> players = world.getPlayers();

	for (auto player : players) {
		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(player->entity);

		InputState& inputState = player->inputState;
		float moveX =
			inputState.moveLeft && inputState.moveRight ? 0 :
			inputState.moveLeft ? -1 :
			inputState.moveRight ? 1 :
			0;

		float moveY =
			inputState.moveUp && inputState.moveDown ? 0 :
			inputState.moveUp ? -1 :
			inputState.moveDown ? 1 :
			0;

		glm::vec2 direction(moveX, moveY);
		physicsSystem.setDirection(physicsCmpnt, direction);
	}
}
}
