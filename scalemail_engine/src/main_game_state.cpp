#include "gui/gui.hpp"
#include "ai_system.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "main_game_state.hpp"
#include "map.hpp"
#include "player.hpp"
#include "player_camera.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>

namespace ScaleMail
{
//	============================================================================
MainGameState::MainGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager) {
}

//	============================================================================
void MainGameState::activate(Game& game) {
	World& world = *game.world;

	game.cameraFollow = true;

	activatePlayerCamera(game);

	game.gui->showPlayerHud(true);

	world.getAiSystem().enable(true);

	game.gameWindow.setCursorVisible(true);
}

//	============================================================================
void MainGameState::draw([[maybe_unused]] const Game& game) {
}

//	============================================================================
void MainGameState::initialize([[maybe_unused]] Game& game) {
}

//	============================================================================
void MainGameState::update(Game& game, [[maybe_unused]] float elapsedSeconds) {
	Camera& camera = *game.camera;
	World& world = *game.world;

	std::vector<Player*> players = world.getPlayers();

	for (const auto player : players) {
		if (player->inputDevice == nullptr) {
			continue;
		}

		InputState& inputState = player->inputDevice->getInputState();

		HealthSystem& healthSystem = world.getHealthSystem();
		HealthComponent healthCmpnt = healthSystem.getComponent(player->entity);
		HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);

		bool fire = false;
		float moveX = 0.0f;
		float moveY = 0.0f;
		glm::vec2 gunTarget;

		if (!healthGauge.isEmpty()) {

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
		if (gunSystem.hasComponent(player->entity)) {
			GunComponent gunCmpnt = gunSystem.getComponent(player->entity);

			gunSystem.setTarget(gunCmpnt, gunTarget);
			gunSystem.setFire(gunCmpnt, fire);
		}

		//	Use items
		InventorySystem& inventorySystem = world.getInventorySystem();

		InventoryComponent inventoryCmpnt =
			inventorySystem.getComponent(player->entity);

		auto& items = inventorySystem.getItems(inventoryCmpnt);

		for (size_t n = 0; n < InputState::USE_ITEM_COUNT; ++n) {
			if (inputState.useItem[n] && n < items.size()) {
				std::shared_ptr<Item> item = items[n];
				inventorySystem.useItem(inventoryCmpnt, item);
			}

			inputState.useItem[n] = false;
		}
	}
}
}
