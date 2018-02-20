#include "entity.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
Entity createProp(World& world, const glm::vec2& position,
				  const int frame1TilesetId, int frame2TilesetId, bool decal) {
	Entity entity = world.createEntity();

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setDecal(spriteCmpnt, decal);
	spriteSystem.setTileset(spriteCmpnt, "world");
	spriteSystem.setTilesetId(spriteCmpnt,
							  { frame1TilesetId, frame2TilesetId });

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, position + glm::vec2(8.0f, -8.0f));
	physicsSystem.setRadius(physicsCmpnt, 0);

	return entity;
}

//  ============================================================================
Entity createProp(World& world, const glm::vec2& position, const int tilesetId,
				  const bool decal) {
	return createProp(world, position, tilesetId, tilesetId, decal);
}
}