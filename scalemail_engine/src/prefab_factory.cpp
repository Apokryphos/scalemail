#include "bullet_util.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "light_system.hpp"
#include "loot_system.hpp"
#include "particle_system.hpp"
#include "prefab_factory.hpp"
#include "physics_system.hpp"
#include "random.hpp"
#include "sprite_system.hpp"
#include "string_util.hpp"
#include "world.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
//  ============================================================================
static void buildAlly(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setMaxSpeed(physicsCmpnt, 16.0f);

	int bulletIndex = world.getRandom().nextInt(0, 3);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setBulletImpactTilesetId(gunCmpnt, getBulletImpactTilesetId(bulletIndex));
	gunSystem.setBulletTilesetId(gunCmpnt, getBulletTilesetId(bulletIndex));
	gunSystem.setBulletLightColor(gunCmpnt, getBulletLightColor(bulletIndex));
	gunSystem.setCooldownDuration(gunCmpnt, 0.4f);
}

//  ============================================================================
static void buildBat(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setMaxSpeed(physicsCmpnt, 48.0f);

	HealthSystem& healthSystem = world.getHealthSystem();
	HealthComponent healthCmpnt = healthSystem.getComponent(entity);
	HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
	healthGauge.setMax(50.0f);
}

//  ============================================================================
static void buildBlob(Entity entity, World& world) {
	SpriteSystem& spriteSystem = world.getSpriteSystem();
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	spriteSystem.setColor(spriteCmpnt, glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	world.getInventorySystem().addComponent(entity);
}

//  ============================================================================
static void buildHealthPotion(Entity entity, World& world) {
	SpriteSystem& spriteSystem = world.getSpriteSystem();
	SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);
	int tilesetId = spriteSystem.getTilesetId(spriteCmpnt);

	Item item = {};
	item.heal = 50.0f;
	item.tilesetId = tilesetId;

	LootSystem& lootSystem = world.getLootSystem();
	LootComponent lootCmpnt = lootSystem.getComponent(entity);
	world.getLootSystem().setItem(lootCmpnt, item);
}

//  ============================================================================
static void buildSkeleton(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setMaxSpeed(physicsCmpnt, 32.0f);

	HealthSystem& healthSystem = world.getHealthSystem();
	HealthComponent healthCmpnt = healthSystem.getComponent(entity);
	HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
	healthGauge.setMax(30.0f);
}

//  ============================================================================
static void buildSkeletonWarrior(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setMaxSpeed(physicsCmpnt, 32.0f);

	HealthSystem& healthSystem = world.getHealthSystem();
	HealthComponent healthCmpnt = healthSystem.getComponent(entity);
	HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
	healthGauge.setMax(30.0f);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setBulletImpactTilesetId(gunCmpnt, getBulletImpactTilesetId(4));
	gunSystem.setBulletTilesetId(gunCmpnt, getBulletTilesetId(4));
	gunSystem.setBulletLightColor(gunCmpnt, getBulletLightColor(4));
	gunSystem.setBulletRotateOffset(gunCmpnt, glm::radians(135.0f));
	gunSystem.setCooldownDuration(gunCmpnt, 1.5f);
}

//  ============================================================================
static void buildVampire(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setMaxSpeed(physicsCmpnt, 50.0f);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setCooldownDuration(gunCmpnt, 0.25f);
}

//  ============================================================================
static void buildWisp(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setIgnoreActorCollisions(physicsCmpnt, true);
	physicsSystem.setMaxSpeed(physicsCmpnt, 20.0f);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setCooldownDuration(gunCmpnt, 0.5f);

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);
	LightComponent lightCmpnt = lightSystem.getComponent(entity);

	glm::vec4 lightColor = getBulletLightColor(0);

	lightSystem.setOffset(lightCmpnt, glm::vec2(0, -2.0f));
	lightSystem.setColor(lightCmpnt, lightColor);
	lightSystem.setGlowSize(lightCmpnt, glm::vec2(8.0f));
	lightSystem.setSize(lightCmpnt, glm::vec2(64.0f));
	lightSystem.setPulse(lightCmpnt, 8);
	lightSystem.setPulseSize(lightCmpnt, 4);

	ParticleSystem& particleSystem = world.getParticleSystem();
	particleSystem.addComponent(entity);

	ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

	ParticleComponentData emitter = {};
	emitter.life = 1.6f;
	emitter.decay = 1.0f;
	emitter.duration = 1.0f;
	emitter.emitCount = 4;
	emitter.interval = 0.16f;
	emitter.minSize = 0.5f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = 4.0f;
	emitter.maxSpeed = 8.0f;
	emitter.spread = 0.349066f;
	emitter.direction = glm::vec3(0.0f, -1.0f, 2.0f);
	emitter.color = lightColor;
	emitter.width = 4.0f;

	particleSystem.setData(particleCmpnt, emitter);
}

//  ============================================================================
void PrefabFactory::buildPrefab(Entity entity, std::string prefabName,
								World& world) {
	prefabName = toLowercase(prefabName);

	if (prefabName == "ally") {
		buildAlly(entity, world);
	} else if (prefabName == "bat") {
		buildBat(entity, world);
	} else if (prefabName == "blob") {
		buildBlob(entity, world);
	} else if (prefabName == "skeleton") {
		buildSkeleton(entity, world);
	} else if (prefabName == "skeleton_warrior") {
		buildSkeletonWarrior(entity, world);
	} else if (prefabName == "vampire") {
		buildVampire(entity, world);
	} else if (prefabName == "wisp") {
		buildWisp(entity, world);
	} else if (prefabName == "health_potion") {
		buildHealthPotion(entity, world);
	}
}
}