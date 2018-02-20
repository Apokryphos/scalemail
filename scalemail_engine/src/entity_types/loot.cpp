#include "entity.hpp"
#include "item.hpp"
#include "loot_system.hpp"
#include "light_system.hpp"
#include "math_util.hpp"
#include "name_system.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include "sprite_effect_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
Entity createLoot(World& world, const glm::vec2& position,
				  const glm::vec2& size, const int tilesetId,
				  const std::string& name, const std::string& prefab) {
	Entity entity = world.createEntity();

	Item item = {};
	item.tilesetId = tilesetId;

	LootSystem& lootSystem = world.getLootSystem();
	lootSystem.addComponent(entity);
	LootComponent lootCmpnt = lootSystem.getComponent(entity);
	lootSystem.setItem(lootCmpnt, item);

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setTileset(spriteCmpnt, "items");
	spriteSystem.setTilesetId(spriteCmpnt, { tilesetId });
	spriteSystem.setSize(spriteCmpnt, size);

	SpriteEffectSystem& spriteEffectSystem = world.getSpriteEffectSystem();
	spriteEffectSystem.addComponent(entity);
	SpriteEffectComponent spriteEffectCmpnt =
		spriteEffectSystem.getComponent(entity);
	spriteEffectSystem.setBlinkDuration(spriteEffectCmpnt, 0.75f);
	spriteEffectSystem.bob(spriteEffectCmpnt, 2.5f);

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, glm::vec2(position.x + 8.0f, position.y - 8.0f));
	physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::ITEM);
	physicsSystem.setRadius(physicsCmpnt, size.x * 0.5f);

	glm::vec4 lightColor(0.8706f, 0.1373f, 0.1373f, 1.0f);
	const float lightSize = size.x * 1.5f;
	const float lightGlowSize = size.x * 0.5f;
	const float lightPulse = 16;
	const float lightPulseSize = 3;

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);
	LightComponent lightCmpnt = lightSystem.getComponent(entity);
	lightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
	lightSystem.setColor(lightCmpnt, lightColor);
	lightSystem.setGlowSize(lightCmpnt, glm::vec2(lightGlowSize));
	lightSystem.setSize(lightCmpnt, glm::vec2(lightSize));
	lightSystem.setPulse(lightCmpnt, lightPulse);
	lightSystem.setPulseSize(lightCmpnt, lightPulseSize);

	ParticleSystem& particleSystem = world.getParticleSystem();
	particleSystem.addComponent(entity);
	ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

	lightColor.a = 0.5f;

	ParticleComponentData emitter = {};
	emitter.life = 2.9f;
	emitter.decay = 1.0f;
	emitter.duration = 1.0f;
	emitter.emitCount = 4;
	emitter.delay = 0.0f;
	emitter.interval = 0.1f;
	emitter.minSize = 0.25f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = 6.0f;
	emitter.maxSpeed = 10.0f;
	emitter.spread = toRadians(45.0f);
	emitter.direction = glm::vec3(0.0f, -1.5f, 2.0f);
	emitter.color = lightColor;
	emitter.height = 24.0f;
	emitter.width = 8.0f;
	emitter.radius = 8.0f;

	particleSystem.setData(particleCmpnt, emitter);

	if (name != "") {
		NameSystem& nameSystem = world.getNameSystem();
		nameSystem.addComponent(entity);
		NameComponent nameCmpnt = nameSystem.getComponent(entity);
		nameSystem.setName(nameCmpnt, name);
	}

	if (prefab != "") {
		world.getPrefabFactory().buildPrefab(entity, prefab, world);
	}

	return entity;
}
}