#include "bullet_util.hpp"
#include "direction.hpp"
#include "entity_types.hpp"
#include "map.hpp"
#include "map_load.hpp"
#include "math_util.hpp"
#include "sprite.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
World::World() : mPhysicsSystem(mEntityManager), mSpriteSystem(mEntityManager),
				 mLightSystem(mEntityManager),   mNameSystem(mEntityManager),
				 mBulletSystem(mEntityManager),  mExpireSystem(mEntityManager),
				 mTriggerSystem(mEntityManager), mGunSystem(mEntityManager),
				 mDoorSystem(mEntityManager),	 mAiSystem(mEntityManager),
				 mHealthSystem(mEntityManager),  mDamageSystem(mEntityManager),
				 mSpriteEffectSystem(mEntityManager),
				 mBurySystem(mEntityManager), mParticleSystem(mEntityManager),
				 mLootSystem(mEntityManager), mInventorySystem(mEntityManager) {
	mPlayers.emplace_back("Player1");
	mPlayers.emplace_back("Player2");
	mPlayers.emplace_back("Player3");
	mPlayers.emplace_back("Player4");

	mPhysicsSystem.addEntityCollisionCallback(
		std::bind(&BulletSystem::onEntityCollision, &mBulletSystem,
				  std::placeholders::_1));

	mPhysicsSystem.addStaticCollisionCallback(
		std::bind(&BulletSystem::onStaticCollision, &mBulletSystem,
				  std::placeholders::_1));

	mPhysicsSystem.addEntityCollisionCallback(
		std::bind(&LootSystem::onEntityCollision, &mLootSystem,
				  std::placeholders::_1));
}

//  ============================================================================
Entity World::createEntity() {
	return mEntityManager.createEntity();
}

//  ============================================================================
void World::destroyBullet(Entity entity) {
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	glm::vec2 bulletPosition = mPhysicsSystem.getPosition(physicsCmpnt);
	float bulletSpeed = mPhysicsSystem.getSpeed(physicsCmpnt);

	BulletComponent bulletCmpnt = mBulletSystem.getComponent(entity);
	int impactTilesetId = mBulletSystem.getImpactTilesetId(bulletCmpnt);

	LightComponent lightCmpnt = mLightSystem.getComponent(entity);
	glm::vec4 lightColor = mLightSystem.getColor(lightCmpnt);

	//	Destroy bullet entity
	this->destroyEntity(entity);

	//	Create bullet impact effect entity
	Entity fxEntity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(fxEntity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(fxEntity);
	mSpriteSystem.setTileset(spriteCmpnt, "fx");
	mSpriteSystem.setTilesetId(
		spriteCmpnt,
		{ impactTilesetId, impactTilesetId + 1, impactTilesetId + 2 });
	mSpriteSystem.setAnimationDuration(spriteCmpnt, 0.3f);

	mPhysicsSystem.addComponent(fxEntity);
	physicsCmpnt = mPhysicsSystem.getComponent(fxEntity);
	mPhysicsSystem.setPosition(physicsCmpnt, bulletPosition);
	mPhysicsSystem.setRadius(physicsCmpnt, 0);

	mExpireSystem.addComponent(fxEntity);
	ExpireComponent expireCmpnt = mExpireSystem.getComponent(fxEntity);
	mExpireSystem.setDuration(expireCmpnt, 0.3f);

	mLightSystem.addComponent(fxEntity);
	lightCmpnt = mLightSystem.getComponent(fxEntity);
	mLightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
	mLightSystem.setColor(lightCmpnt, lightColor);
	mLightSystem.setGlowSize(lightCmpnt, glm::vec2(0));
	mLightSystem.setSize(lightCmpnt, glm::vec2(48));

	mParticleSystem.addComponent(fxEntity);
	ParticleComponent particleCmpnt = mParticleSystem.getComponent(fxEntity);

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

	mParticleSystem.setData(particleCmpnt, emitter);
}

//  ============================================================================
void World::destroyEntity(Entity entity) {
	if (mAiSystem.hasComponent(entity)) {
		mAiSystem.removeComponent(entity);
	}

	if (mBulletSystem.hasComponent(entity)) {
		mBulletSystem.removeComponent(entity);
	}

	if (mBurySystem.hasComponent(entity)) {
		mBurySystem.removeComponent(entity);
	}

	if (mDamageSystem.hasComponent(entity)) {
		mDamageSystem.removeComponent(entity);
	}

	if (mDoorSystem.hasComponent(entity)) {
		mDoorSystem.removeComponent(entity);
	}

	if (mExpireSystem.hasComponent(entity)) {
		mExpireSystem.removeComponent(entity);
	}

	if (mGunSystem.hasComponent(entity)) {
		mGunSystem.removeComponent(entity);
	}

	if (mHealthSystem.hasComponent(entity)) {
		mHealthSystem.removeComponent(entity);
	}

	if (mInventorySystem.hasComponent(entity)) {
		mInventorySystem.removeComponent(entity);
	}

	if (mLightSystem.hasComponent(entity)) {
		mLightSystem.removeComponent(entity);
	}

	if (mLootSystem.hasComponent(entity)) {
		mLootSystem.removeComponent(entity);
	}

	if (mNameSystem.hasComponent(entity)) {
		mNameSystem.removeComponent(entity);
	}

	if (mParticleSystem.hasComponent(entity)) {
		mParticleSystem.removeComponent(entity);
	}

	if (mPhysicsSystem.hasComponent(entity)) {
		mPhysicsSystem.removeComponent(entity);
	}

	if (mSpriteSystem.hasComponent(entity)) {
		mSpriteSystem.removeComponent(entity);
	}

	if (mSpriteEffectSystem.hasComponent(entity)) {
		mSpriteEffectSystem.removeComponent(entity);
	}

	if (mTriggerSystem.hasComponent(entity)) {
		mTriggerSystem.removeComponent(entity);
	}

	mEntityManager.destroyEntity(entity);
}

//  ============================================================================
AiBehaviorFactory& World::getAiBehaviorFactory() {
	return mAiBehaviorFactory;
}

//  ============================================================================
AiSystem& World::getAiSystem() {
	return mAiSystem;
}

//  ============================================================================
BulletSystem& World::getBulletSystem() {
	return mBulletSystem;
}

//  ============================================================================
BurySystem& World::getBurySystem() {
	return mBurySystem;
}

//  ============================================================================
DamageSystem& World::getDamageSystem() {
	return mDamageSystem;
}

//  ============================================================================
DoorSystem& World::getDoorSystem() {
	return mDoorSystem;
}

//	============================================================================
std::vector<Entity> World::getEntitiesByName(const std::string name) const {
	return mNameSystem.getEntitiesByName(name);
}

//  ============================================================================
GunSystem& World::getGunSystem() {
	return mGunSystem;
}

//  ============================================================================
HealthSystem& World::getHealthSystem() {
	return mHealthSystem;
}

//  ============================================================================
InventorySystem& World::getInventorySystem() {
	return mInventorySystem;
}

//  ============================================================================
LightSystem& World::getLightSystem() {
	return mLightSystem;
}

//  ============================================================================
LootSystem& World::getLootSystem() {
	return mLootSystem;
}

//  ============================================================================
ParticleSystem& World::getParticleSystem() {
	return mParticleSystem;
}

//  ============================================================================
std::vector<Player*> World::getPlayers() {
	std::vector<Player*> players;

	for (Player& player : mPlayers) {
		players.push_back(&player);
	}

	return players;
}

//  ============================================================================
PrefabFactory& World::getPrefabFactory() {
	return mPrefabFactory;
}

//  ============================================================================
Map* World::getMap() {
	return mMap.get();
}

//  ============================================================================
NameSystem& World::getNameSystem() {
	return mNameSystem;
}

//  ============================================================================
PhysicsSystem& World::getPhysicsSystem() {
	return mPhysicsSystem;
}

//  ============================================================================
Random& World::getRandom() {
	return mRandom;
}

//  ============================================================================
SpriteEffectSystem& World::getSpriteEffectSystem() {
	return mSpriteEffectSystem;
}

//  ============================================================================
SpriteSystem& World::getSpriteSystem() {
	return mSpriteSystem;
}

//  ============================================================================
TriggerSystem& World::getTriggerSystem() {
	return mTriggerSystem;
}

//  ============================================================================
void World::initialize(AssetManager* assetManager) {
	mSpriteSystem.initialize(assetManager);
	mLightSystem.initialize(*assetManager);
	mDoorSystem.initialize(mPhysicsSystem, mSpriteSystem);
	mPhysicsSystem.initialize(*assetManager);
	mBulletSystem.initialize(mDamageSystem);
	mBurySystem.initialize(mRandom, mPhysicsSystem, mSpriteSystem);
	mParticleSystem.initialize(mRandom);
	mAiSystem.initialize(*assetManager);
	mLootSystem.initialize(mInventorySystem);
}

//  ============================================================================
bool World::entityIsAlive(const Entity& entity) {
	return mEntityManager.isAlive(entity);
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
	mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);

	std::vector<PlayerStart> playerStarts = mMap->getPlayerStarts();

	assert(playerStarts.size() == 4);
	assert(mPlayers.size() == 4);

	for (size_t p = 0; p < mPlayers.size(); ++p) {
		PlayerStart& playerStart = playerStarts[p];

		Entity entity = createPlayerActor(
			*this,
			playerStart.position,
			playerStart.actorIndex,
			playerStart.facing,
			"Player" + (p + 1));

		mPlayers[p].entity = entity;
	}
}

//  ============================================================================
void World::update(float elapsedSeconds) {
	mBulletSystem.simulate(*this, elapsedSeconds);

	mAiSystem.update(*this, elapsedSeconds);

	mPhysicsSystem.simulate(elapsedSeconds);
	mSpriteSystem.update(elapsedSeconds, mPhysicsSystem);
	mLightSystem.update(elapsedSeconds, mPhysicsSystem);

	mPhysicsSystem.update();

	mGunSystem.update(*this, elapsedSeconds);

	mTriggerSystem.update(
		mNameSystem,
		mPhysicsSystem,
		mDoorSystem);

	mDamageSystem.applyDamage(mHealthSystem, mSpriteEffectSystem);

	mSpriteEffectSystem.update(elapsedSeconds, mSpriteSystem);

	mBurySystem.update(elapsedSeconds);

	mHealthSystem.update(*this);

	mExpireSystem.update(*this, elapsedSeconds);

	mParticleSystem.update(mPhysicsSystem, elapsedSeconds);

	mLootSystem.simulate(*this, elapsedSeconds);
}
}