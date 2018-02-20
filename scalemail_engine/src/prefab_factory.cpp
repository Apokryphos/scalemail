#include "bullet_util.hpp"
#include "gun_system.hpp"
#include "prefab_factory.hpp"
#include "sprite_system.hpp"
#include "string_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
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
	physicsSystem.setSpeed(physicsCmpnt, 32.0f);

	HealthSystem& healthSystem = world.getHealthSystem();
	HealthComponent healthCmpnt = healthSystem.getComponent(entity);
	HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
	healthGauge.setMax(30.0f);
}

//  ============================================================================
static void buildVampire(Entity entity, World& world) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setSpeed(physicsCmpnt, 50.0f);

	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setBulletImpactTilesetId(gunCmpnt, getBulletImpactTilesetId(3));
	gunSystem.setBulletTilesetId(gunCmpnt, getBulletTilesetId(3));
	gunSystem.setBulletLightColor(gunCmpnt, getBulletLightColor(3));
	gunSystem.setCooldownDuration(gunCmpnt, 0.25f);
}

//  ============================================================================
void PrefabFactory::buildPrefab(Entity entity, std::string prefabName,
								World& world) {
	prefabName = toLowercase(prefabName);

	if (prefabName == "blob") {
		buildBlob(entity, world);
	} else if (prefabName == "skeleton") {
		buildSkeleton(entity, world);
	} else if (prefabName == "vampire") {
		buildVampire(entity, world);
	} else if (prefabName == "healthpotion") {
		buildHealthPotion(entity, world);
	}
}
}