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
static Entity getPlayerCameraEntity(World& world) {
	auto entities =
		world.getNameSystem().getEntitiesByName(PLAYER_CAMERA_NAME);

	if (entities.size() == 0) {
		return {};
	}

	return entities[0];
}

//	============================================================================
void setPlayerCameraFollowEntity(Entity entity, Game& game, bool activateCamera) {
	World& world = *(game.world);

	Entity cameraEntity = getPlayerCameraEntity(world);

	CameraSystem& cameraSystem = world.getCameraSystem();

	if (!cameraSystem.hasComponent(cameraEntity)) {
		return;
	}

	const CameraComponent cameraCmpnt =
		cameraSystem.getComponent(cameraEntity);

	cameraSystem.followEntity(cameraCmpnt, entity);

	if (activateCamera) {
		//	Set camera as active camera
		game.camera = &(cameraSystem.getCamera(cameraCmpnt));
	}
}

//	============================================================================
void activatePlayerCamera(Game& game) {
	World& world = *(game.world);

	Entity playerEntity = world.getPlayers()[0]->entity;

	setPlayerCameraFollowEntity(playerEntity, game, true);
}

//	============================================================================
void createPlayerCamera(World& world) {
	createCamera(world, PLAYER_CAMERA_NAME);
}
}