#include "camera.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "main_game_state.hpp"
#include "light_system.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>

namespace ScaleMail
{
const float cameraStartPosition = 1024.0f;
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
void MainGameState::draw([[maybe_unused]] const Game& game,
						 [[maybe_unused]] Camera& camera) {
}

//	============================================================================
void MainGameState::initialize([[maybe_unused]] Game& game) {
}

//	============================================================================
void MainGameState::update(World& world,
						   [[maybe_unused]] Camera& camera,
						   [[maybe_unused]] float elapsedSeconds) {
	std::vector<Player*> players = world.getPlayers();

	for (auto player : players) {
		InputState& inputState = player->inputState;

		SpriteSystem& spriteSystem = world.getSpriteSystem();
		SpriteComponent spriteCmpnt = spriteSystem.getComponent(player->entity);

		//	Update sprite facing
		if (inputState.moveLeft || inputState.moveRight) {
			if (inputState.moveLeft && !inputState.moveRight) {
				spriteSystem.setFacing(spriteCmpnt, Direction::WEST);
			} else if (inputState.moveRight && !inputState.moveLeft) {
				spriteSystem.setFacing(spriteCmpnt, Direction::EAST);
			}
		}

		if (inputState.moveUp || inputState.moveDown) {
			if (inputState.moveUp && !inputState.moveDown) {
				spriteSystem.setFacing(spriteCmpnt, Direction::NORTH);
			} else if (inputState.moveDown && !inputState.moveUp) {
				spriteSystem.setFacing(spriteCmpnt, Direction::SOUTH);
			}
		}

		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(player->entity);

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

		//	Fire bullets
		if (inputState.fire) {
			const int bulletTilesetId = 32;
			const float bulletSpeed = 128.0f;

			glm::vec2 target = camera.unproject(inputState.aimPosition);
			glm::vec2 origin = physicsSystem.getPosition(physicsCmpnt);
			glm::vec2 bulletDirection = target - origin;

			if (glm::length2(bulletDirection) > 1) {
				bulletDirection = glm::normalize(bulletDirection);
			}

			world.createBullet(
				origin,
				bulletDirection,
				bulletSpeed,
				bulletTilesetId);
		}
	}
}
}
