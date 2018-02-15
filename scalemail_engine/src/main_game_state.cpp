#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "gun_system.hpp"
#include "main_game_state.hpp"
#include "map.hpp"
#include "physics_system.hpp"
#include "rectangle.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>

namespace ScaleMail
{
//	============================================================================
static void updateCameraPosition(Game& game) {
	World* world = game.world;
	Map* map = world->getMap();
	Camera* camera = game.camera;

	if (map != nullptr && camera != nullptr) {
		std::vector<Player*> players = world->getPlayers();

		if (game.cameraFollow && players.size() > 0) {
			//	Camera follows first player
			PhysicsSystem& physicsSystem = world->getPhysicsSystem();
			PhysicsComponent physicsCmpnt = physicsSystem.getComponent(players[0]->entity);
			glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

			std::vector<Rectangle> cameraBounds;

			const MapCamera* mapCamera = map->getCamera("PlayerCamera");

			if (mapCamera != nullptr) {
				cameraBounds = mapCamera->bounds;
			}

			auto findBounds = std::find_if(cameraBounds.begin(), cameraBounds.end(),
				[position](const Rectangle& b) -> bool {
					return b.contains(position);
				}
			);

			if (findBounds != cameraBounds.end()) {
				Rectangle bounds = *findBounds;
				camera->setBounds(bounds);
			}

			camera->setPosition(position);
		}
	}
}

//	============================================================================
MainGameState::MainGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager) {
}

//	============================================================================
void MainGameState::activate(Game& game) {
	game.cameraFollow = true;
	updateCameraPosition(game);

	game.gui->showPlayerHud(true);
}

//	============================================================================
void MainGameState::draw([[maybe_unused]] const Game& game,
						 [[maybe_unused]] Camera& camera) {
}

//	============================================================================
void MainGameState::initialize([[maybe_unused]] Game& game) {
}

//	============================================================================
void MainGameState::update(Game& game, [[maybe_unused]] float elapsedSeconds) {
	Camera& camera = *game.camera;
	World& world = *game.world;

	updateCameraPosition(game);

	std::vector<Player*> players = world.getPlayers();

	for (auto player : players) {
		HealthSystem& healthSystem = world.getHealthSystem();
		HealthComponent healthCmpnt = healthSystem.getComponent(player->entity);
		HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);

		bool fire = false;
		float moveX = 0.0f;
		float moveY = 0.0f;
		glm::vec2 gunTarget;

		if (!healthGauge.isEmpty()) {
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

			//	Update direction
			moveX =
				inputState.moveLeft && inputState.moveRight ? 0 :
				inputState.moveLeft ? -1 :
				inputState.moveRight ? 1 :
				0;

			moveY =
				inputState.moveUp && inputState.moveDown ? 0 :
				inputState.moveUp ? -1 :
				inputState.moveDown ? 1 :
				0;

			//	Gun firing and reticle
			fire = inputState.fire;
			gunTarget = camera.unproject(inputState.aimPosition);
		}

		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(player->entity);

		glm::vec2 direction(moveX, moveY);
		physicsSystem.setDirection(physicsCmpnt, direction);

		//	Update gun
		GunSystem& gunSystem = world.getGunSystem();
		GunComponent gunCmpnt = gunSystem.getComponent(player->entity);

		gunSystem.setTarget(gunCmpnt, gunTarget);
		gunSystem.setFire(gunCmpnt, fire);
	}
}
}
