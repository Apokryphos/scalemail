#include "gui/gui.hpp"
#include "ai_system.hpp"
#include "ambient_light.hpp"
#include "bury_system.hpp"
#include "camera.hpp"
#include "camera_system.hpp"
#include "entity_types.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "main_game_state.hpp"
#include "map.hpp"
#include "player.hpp"
#include "physics_system.hpp"
#include "name_system.hpp"
#include "rectangle.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <vector>

namespace ScaleMail
{
//	============================================================================
void getPlayerCamera(World& world, Entity& cameraEntity, Camera*& camera) {
	camera = nullptr;

	NameSystem& nameSystem = world.getNameSystem();

	auto cameraEntities = nameSystem.getEntitiesByName("PlayerCamera");

	if (cameraEntities.size() > 0) {
		cameraEntity = cameraEntities[0];

		CameraSystem& cameraSystem = world.getCameraSystem();

		const CameraComponent cameraCmpnt =
			cameraSystem.getComponent(cameraEntity);

		camera = &(cameraSystem.getCamera(cameraCmpnt));
	}
}

//	============================================================================
MainGameState::MainGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager) {
}

//	============================================================================
void MainGameState::activate(Game& game) {
	World& world = *game.world;

	game.cameraFollow = true;

	Camera* camera = nullptr;
	Entity cameraEntity;
	getPlayerCamera(world, cameraEntity, camera);

	CameraSystem& cameraSystem = world.getCameraSystem();

	if (cameraSystem.hasComponent(cameraEntity)) {
		game.camera = camera;

		const CameraComponent cameraCmpnt =
			cameraSystem.getComponent(cameraEntity);

		Entity playerEntity = world.getPlayers()[0]->entity;

		cameraSystem.followEntity(cameraCmpnt, playerEntity);
	}

	game.gui->showPlayerHud(true);

	//	Unearth skeletons
	NameSystem& nameSystem = world.getNameSystem();
	std::vector<Entity> buriedEntities = nameSystem.getEntitiesByName("Skeleton");

	BurySystem& burySystem = world.getBurySystem();
	for (const auto entity : buriedEntities) {
		if (burySystem.hasComponent(entity)) {
			BuryComponent buryCmpnt = burySystem.getComponent(entity);

			if (burySystem.getBuryState(buryCmpnt) == BuryState::BURIED) {
				world.getBurySystem().rise(buryCmpnt, false);
			}
		}
	}

	world.getAiSystem().enable(true);

	game.gameWindow.setCursorVisible(true);
}

//	============================================================================
void MainGameState::draw([[maybe_unused]] const Game& game,
						 [[maybe_unused]] Camera& camera) {
}

//	============================================================================
void MainGameState::initialize(Game& game) {
	World& world = *game.world;
	createCamera(world, "PlayerCamera");
}

//	============================================================================
void MainGameState::update(Game& game, [[maybe_unused]] float elapsedSeconds) {
	Camera& camera = *game.camera;
	World& world = *game.world;

	std::vector<Player*> players = world.getPlayers();

	for (const auto player : players) {
		HealthSystem& healthSystem = world.getHealthSystem();
		HealthComponent healthCmpnt = healthSystem.getComponent(player->entity);
		HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);

		bool fire = false;
		float moveX = 0.0f;
		float moveY = 0.0f;
		glm::vec2 gunTarget;

		if (!healthGauge.isEmpty()) {
			InputState& inputState = player->inputState;

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
