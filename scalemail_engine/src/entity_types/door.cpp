#include "door_system.hpp"
#include "entity.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include "world.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
//  ============================================================================
Entity createDoor(World& world, const glm::vec2& position,
				  const int openTilesetId, const int closedTilesetId,
				  const bool open, const std::string& name) {
	Entity entity = world.createEntity();

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, position);
	physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::STATIC);

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setTileset(spriteCmpnt, "world");
	spriteSystem.setTilesetId(spriteCmpnt,
							  { open ? openTilesetId : closedTilesetId });

	DoorSystem& doorSystem = world.getDoorSystem();
	doorSystem.addComponent(entity);
	DoorComponent doorCmpnt = doorSystem.getComponent(entity);
	doorSystem.setTilesetId(doorCmpnt, openTilesetId, closedTilesetId);
	doorSystem.setOpen(doorCmpnt, open);

	if (name != "") {
		NameSystem& nameSystem = world.getNameSystem();
		nameSystem.addComponent(entity);
		NameComponent nameCmpnt = nameSystem.getComponent(entity);
		nameSystem.setName(nameCmpnt, name);
	}

	return entity;
}

//  ============================================================================
Entity createDoor(World& world, const glm::vec2& position,
				  const int openTilesetId, const int closedTilesetId,
				  const int childOpenTilesetId,
				  const int childClosedTilesetId, const bool open,
				  const std::string& name) {
	Entity entity = createDoor(world, position, openTilesetId, closedTilesetId,
									 open, name);

	DoorSystem& doorSystem = world.getDoorSystem();
	DoorComponent doorCmpnt = doorSystem.getComponent(entity);
	doorSystem.setChildTilesetId(doorCmpnt, childOpenTilesetId,
								 childClosedTilesetId);

	return entity;
}
}