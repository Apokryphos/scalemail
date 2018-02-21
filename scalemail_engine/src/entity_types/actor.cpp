#include "ai_behavior.hpp"
#include "ai_system.hpp"
#include "damage_system.hpp"
#include "entity_util.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "sprite_effect_system.hpp"
#include "sprite_system.hpp"
#include "world.hpp"


namespace ScaleMail
{
//  ============================================================================
Entity createActor(World& world, const glm::vec2& position,
				   const glm::vec2& size, const int actorIndex,
				   const Direction facing, const std::string& name,
				   const std::string& prefab, const std::string& ai) {
	Entity entity = world.createEntity();

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setTileset(spriteCmpnt, "actors");
	spriteSystem.setActorIndex(spriteCmpnt, actorIndex);
	spriteSystem.setFacing(spriteCmpnt, facing);
	spriteSystem.setOffsetY(spriteCmpnt, -4.0f);
	spriteSystem.setSize(spriteCmpnt, size);

	SpriteEffectSystem& spriteEffectSystem = world.getSpriteEffectSystem();
	spriteEffectSystem.addComponent(entity);
	SpriteEffectComponent spriteEffectCmpnt =
		spriteEffectSystem.getComponent(entity);
	spriteEffectSystem.setBlinkDuration(spriteEffectCmpnt, 0.75f);

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, position);
	physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::ACTOR);
	physicsSystem.setRadius(physicsCmpnt, size.x * 0.25f);

	world.getHealthSystem().addComponent(entity);

	world.getDamageSystem().addComponent(entity);

	world.getGunSystem().addComponent(entity);

	setEntityName(name, entity, world.getNameSystem());

	addAiBehavior(ai, entity, world.getAiSystem(),
				  world.getAiBehaviorFactory());

	addPrefab(prefab, entity, world);

	return entity;
}

//  ============================================================================
Entity createPlayerActor(World& world, const glm::vec2 position,
						 const int actorIndex, const Direction facing,
						 const std::string& name) {
	Entity entity = createActor(world, position, glm::vec2(16.0f, 16.0f),
								actorIndex, facing, name, "", "");

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::PLAYER_ACTOR);

	HealthSystem& healthSystem = world.getHealthSystem();
	HealthComponent healthCmpnt = healthSystem.getComponent(entity);
	healthSystem.setRespawn(healthCmpnt, true);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setBulletDamage(gunCmpnt, 10.0f);
	gunSystem.setCooldownDuration(gunCmpnt, 0.1f);

	world.getInventorySystem().addComponent(entity);

	return entity;
}
}