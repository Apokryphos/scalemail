#include "bullet_data.hpp"
#include "bullet_system.hpp"
#include "entity.hpp"
#include "expire_system.hpp"
#include "light_data.hpp"
#include "light_system.hpp"
#include "math_util.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include "world.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
//  ============================================================================
Entity createBullet(World& world, const Entity& sourceEntity,
					const glm::vec2& position, const BulletData& bulletData,
					const LightData& lightData) {
	Entity entity = world.createEntity();

	BulletSystem& bulletSystem = world.getBulletSystem();
	bulletSystem.addComponent(entity);
	BulletComponent bulletCmpnt = bulletSystem.getComponent(entity);
	bulletSystem.setSourceEntity(bulletCmpnt, sourceEntity);
	bulletSystem.setDamage(bulletCmpnt, bulletData.damage);
	bulletSystem.setImpactTilesetId(bulletCmpnt, bulletData.impactTilesetId);

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setDirection(spriteCmpnt, bulletData.direction);
	spriteSystem.setRotateOffset(spriteCmpnt, bulletData.rotateOffset);
	spriteSystem.setSize(spriteCmpnt, glm::vec2(12.0f, 12.0f));
	spriteSystem.setTileset(spriteCmpnt, "fx");
	spriteSystem.setTilesetId(spriteCmpnt, { bulletData.tilesetId });

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, position);
	physicsSystem.setDirection(physicsCmpnt, bulletData.direction);
	physicsSystem.setRadius(physicsCmpnt, 3.0f);
	physicsSystem.setSpeed(physicsCmpnt, bulletData.speed);

	PhysicsComponent sourcePhysicsCmpnt =
		physicsSystem.getComponent(sourceEntity);

	CollisionGroup sourceGroup =
		physicsSystem.getCollisionGroup(sourcePhysicsCmpnt);

	if (sourceGroup == CollisionGroup::PLAYER_ACTOR) {
		physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::PLAYER_BULLET);
	} else {
		physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::BULLET);
	}

	//	Only add lights if alpha is visible
	if (lightData.color.a > 0.0f) {
		LightSystem& lightSystem = world.getLightSystem();
		lightSystem.addComponent(entity);
		LightComponent lightCmpnt = lightSystem.getComponent(entity);
		lightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
		lightSystem.setColor(lightCmpnt, lightData.color);
		lightSystem.setGlowSize(lightCmpnt, lightData.glowSize);
		lightSystem.setSize(lightCmpnt, lightData.size);
		lightSystem.setPulse(lightCmpnt, lightData.pulse);
		lightSystem.setPulseSize(lightCmpnt, lightData.pulseSize);

		//	Only add particles if alpha is visible
		// ParticleSystem& particleSystem = world.getParticleSystem();
		// particleSystem.addComponent(entity);
		// ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

		// ParticleComponentData emitter = {};
		// emitter.life = 0.6f;
		// emitter.decay = 1.0f;
		// emitter.duration = 1.0f;
		// emitter.emitCount = 1;
		// emitter.delay = 0.2f;
		// emitter.interval = 0.01f;
		// emitter.minSize = 0.5f;
		// emitter.maxSize = 2.0f;
		// emitter.minSpeed = bulletData.speed * 0.25f;
		// emitter.maxSpeed = bulletData.speed * 0.33f;
		// emitter.spread = 0.349066f;
		// emitter.direction = glm::vec3(-1, 1, 0.0f);
		// emitter.height = 4.0f;
		// emitter.width = 4.0f;
		// emitter.direction = glm::vec3(-bulletData.direction, 0.0f);
		// emitter.color = lightData.color;
		// emitter.radius = 2.0f;

		// particleSystem.setData(particleCmpnt, emitter);
	}

	return entity;
}

//  ============================================================================
void destroyBullet(World& world, const Entity& entity) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 bulletPosition = physicsSystem.getPosition(physicsCmpnt);
	float bulletSpeed = physicsSystem.getSpeed(physicsCmpnt);

	BulletSystem& bulletSystem = world.getBulletSystem();
	BulletComponent bulletCmpnt = bulletSystem.getComponent(entity);
	int impactTilesetId = bulletSystem.getImpactTilesetId(bulletCmpnt);

	glm::vec4 lightColor(0.0f, 0.0f, 0.0f, 0.0f);

	LightSystem& lightSystem = world.getLightSystem();
	if (lightSystem.hasComponent(entity)) {
		LightComponent lightCmpnt = lightSystem.getComponent(entity);
		lightColor = lightSystem.getColor(lightCmpnt);
	}

	//	Destroy bullet entity
	world.destroyEntity(entity);

	//	Create bullet impact effect entity
	Entity fxEntity = world.createEntity();

	SpriteSystem& spriteSystem = world.getSpriteSystem();
	spriteSystem.addComponent(fxEntity);
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(fxEntity);
	spriteSystem.setTileset(spriteCmpnt, "fx");
	spriteSystem.setTilesetId(
		spriteCmpnt,
		{ impactTilesetId, impactTilesetId + 1, impactTilesetId + 2 });
	spriteSystem.setAnimationDuration(spriteCmpnt, 0.3f);

	physicsSystem.addComponent(fxEntity);
	physicsCmpnt = physicsSystem.getComponent(fxEntity);
	physicsSystem.setPosition(physicsCmpnt, bulletPosition);
	physicsSystem.setRadius(physicsCmpnt, 0);

	ExpireSystem& expireSystem = world.getExpireSystem();
	expireSystem.addComponent(fxEntity);
	ExpireComponent expireCmpnt = expireSystem.getComponent(fxEntity);
	expireSystem.setDuration(expireCmpnt, 0.3f);

	//	Only add light and particles if alpha is visible
	if (lightColor.a > 0.0f) {
		lightSystem.addComponent(fxEntity);
		LightComponent lightCmpnt = lightSystem.getComponent(fxEntity);
		lightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
		lightSystem.setColor(lightCmpnt, lightColor);
		lightSystem.setGlowSize(lightCmpnt, glm::vec2(0));
		lightSystem.setSize(lightCmpnt, glm::vec2(48));

		ParticleSystem& particleSystem = world.getParticleSystem();
		particleSystem.addComponent(fxEntity);
		ParticleComponent particleCmpnt = particleSystem.getComponent(fxEntity);

		ParticleComponentData emitter = {};
		emitter.life = 0.125f;
		emitter.decay = 1.0f;
		emitter.duration = 1.0f;
		emitter.emitCount = 1;
		emitter.interval = 0.0f;
		emitter.minSize = 0.5f;
		emitter.maxSize = 2.0f;
		emitter.minSpeed = bulletSpeed * 0.1f;
		emitter.maxSpeed = bulletSpeed * 0.2f;
		emitter.spread = TWO_PI;
		emitter.direction = glm::vec3(0.0f, 1.0f, 0.0f);
		emitter.color = lightColor;
		emitter.radius = 8.0f;

		particleSystem.setData(particleCmpnt, emitter);
	}
}
}