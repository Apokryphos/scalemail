#include "camera_system.hpp"
#include "entity_types.hpp"
#include "game.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "world.hpp"

namespace ScaleMail
{
static const std::string PLAYER_CAMERA_NAME = "PlayerCamera";

//	============================================================================
void activatePlayerCamera(Game& game) {
	World& world = *(game.world);

	auto entities =
		world.getNameSystem().getEntitiesByName(PLAYER_CAMERA_NAME);

	if (entities.size() == 0) {
		return;
	}

	Entity entity = entities[0];

	CameraSystem& cameraSystem = world.getCameraSystem();

	if (!cameraSystem.hasComponent(entity)) {
		return;
	}

	const CameraComponent cameraCmpnt =
		cameraSystem.getComponent(entity);

	Entity playerEntity = world.getPlayers()[0]->entity;

	cameraSystem.followEntity(cameraCmpnt, playerEntity);

	//	Set camera as active camera
	game.camera = &(cameraSystem.getCamera(cameraCmpnt));
}

//	============================================================================
void createPlayerCamera(World& world) {
	createCamera(world, PLAYER_CAMERA_NAME);
}
}