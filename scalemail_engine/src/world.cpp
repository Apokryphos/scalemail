#include "ai_behavior.hpp"
#include "bullet_util.hpp"
#include "direction.hpp"
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
				 mBurySystem(mEntityManager), mParticleSystem(mEntityManager) {
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
}

//  ============================================================================
Entity World::createActor(float x, float y, int actorIndex, Direction facing,
						  std::string name, std::string prefab, std::string ai) {
	Entity entity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setTileset(spriteCmpnt, "actors");
	mSpriteSystem.setActorIndex(spriteCmpnt, actorIndex);
	mSpriteSystem.setFacing(spriteCmpnt, facing);
	mSpriteSystem.setOffsetY(spriteCmpnt, -4.0f);

	mSpriteEffectSystem.addComponent(entity);
	SpriteEffectComponent spriteEffectCmpnt =
		mSpriteEffectSystem.getComponent(entity);
	mSpriteEffectSystem.setBlinkDuration(spriteEffectCmpnt, 0.75f);

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, glm::vec2(x + 8.0f, y - 8.0f));
	mPhysicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::ACTOR);
	mPhysicsSystem.setRadius(physicsCmpnt, 4.0f);

	mHealthSystem.addComponent(entity);

	mDamageSystem.addComponent(entity);

	mGunSystem.addComponent(entity);

	if (name != "") {
		mNameSystem.addComponent(entity);
		NameComponent nameCmpnt = mNameSystem.getComponent(entity);
		mNameSystem.setName(nameCmpnt, name);
	}

	if (ai != "") {
		std::shared_ptr<AiBehavior> aiBehavior =
			mAiBehaviorFactory.createAiBehavior(ai);
			aiBehavior->setEntity(entity);

		if (aiBehavior != nullptr) {
			mAiSystem.addComponent(entity);
			AiComponent aiCmpnt = mAiSystem.getComponent(entity);
			mAiSystem.addBehavior(aiCmpnt, aiBehavior);
		}
	}

	if (prefab != "") {
		mPrefabFactory.buildPrefab(entity, prefab, *this);
	}

	return entity;
}

//  ============================================================================
Entity World::createBullet(Entity sourceEntity, glm::vec2 position,
						   glm::vec2 direction, float damage,
						   float speed, int tilesetId, int impactTilesetId,
						   glm::vec4 lightColor) {
	Entity entity = mEntityManager.createEntity();

	mBulletSystem.addComponent(entity);
	BulletComponent bulletCmpnt = mBulletSystem.getComponent(entity);
	mBulletSystem.setDamage(bulletCmpnt, damage);
	mBulletSystem.setSourceEntity(bulletCmpnt, sourceEntity);
	mBulletSystem.setImpactTilesetId(bulletCmpnt, impactTilesetId);

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setDirection(spriteCmpnt, direction);
	mSpriteSystem.setSize(spriteCmpnt, glm::vec2(12.0f, 12.0f));
	mSpriteSystem.setTileset(spriteCmpnt, "fx");
	mSpriteSystem.setTilesetId(spriteCmpnt, { tilesetId });

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, position);
	mPhysicsSystem.setDirection(physicsCmpnt, direction);
	mPhysicsSystem.setRadius(physicsCmpnt, 3.0f);
	mPhysicsSystem.setSpeed(physicsCmpnt, speed);

	PhysicsComponent sourcePhysicsCmpnt =
		mPhysicsSystem.getComponent(sourceEntity);

	CollisionGroup sourceGroup =
		mPhysicsSystem.getCollisionGroup(sourcePhysicsCmpnt);

	if (sourceGroup == CollisionGroup::PLAYER_ACTOR) {
		mPhysicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::PLAYER_BULLET);
	} else {
		mPhysicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::BULLET);
	}

	const float lightSize = 16;
	const float lightGlowSize = lightSize * 0.33f;
	const float lightPulse = 32;
	const float lightPulseSize = 6;

	mLightSystem.addComponent(entity);
	LightComponent lightCmpnt = mLightSystem.getComponent(entity);
	mLightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
	mLightSystem.setColor(lightCmpnt, lightColor);
	mLightSystem.setGlowSize(lightCmpnt, lightGlowSize);
	mLightSystem.setSize(lightCmpnt, lightSize);
	mLightSystem.setPulse(lightCmpnt, lightPulse);
	mLightSystem.setPulseSize(lightCmpnt, lightPulseSize);

	mParticleSystem.addComponent(entity);
	ParticleComponent particleCmpnt = mParticleSystem.getComponent(entity);

	ParticleComponentData emitter = {};
	emitter.life = 0.9f;
	emitter.decay = 1.0f;
	emitter.duration = 1.0f;
	emitter.emitCount = 1;
	emitter.delay = 0.2f;
	emitter.interval = 0.0f;
	emitter.minSize = 0.5f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = speed * 0.25f;
	emitter.maxSpeed = speed * 0.33f;
	emitter.spread = 0.349066f;
	emitter.direction = -direction;
	emitter.color = lightColor;
	emitter.radius = 4.0f;

	mParticleSystem.setData(particleCmpnt, emitter);

	return entity;
}

//  ============================================================================
Entity World::createDoor(float x, float y, int openTilesetId,
						 int closedTilesetId, bool open, const std::string name) {
	Entity entity = mEntityManager.createEntity();

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, glm::vec2(x + 8.0f, y - 8.0f));
	mPhysicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::STATIC);

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setTileset(spriteCmpnt, "world");
	mSpriteSystem.setTilesetId(spriteCmpnt,
							   { open ? openTilesetId : closedTilesetId });

	mDoorSystem.addComponent(entity);
	DoorComponent doorCmpnt = mDoorSystem.getComponent(entity);
	mDoorSystem.setTilesetId(doorCmpnt, openTilesetId, closedTilesetId);
	mDoorSystem.setOpen(doorCmpnt, open);

	if (name != "") {
		mNameSystem.addComponent(entity);
		NameComponent nameCmpnt = mNameSystem.getComponent(entity);
		mNameSystem.setName(nameCmpnt, name);
	}

	return entity;
}

//  ============================================================================
Entity World::createDoor(float x, float y, int openTilesetId,
						 int closedTilesetId, int childOpenTilesetId,
						 int childClosedTilesetId, bool open,
						 const std::string name) {
	Entity entity = this->createDoor(x, y, openTilesetId, closedTilesetId,
									 open, name);

	DoorComponent doorCmpnt = mDoorSystem.getComponent(entity);
	mDoorSystem.setChildTilesetId(doorCmpnt, childOpenTilesetId,
								  childClosedTilesetId);

	return entity;
}

//  ============================================================================
Entity World::createEntity() {
	return mEntityManager.createEntity();
}

//  ============================================================================
Entity World::createPlayerActor(float x, float y, int actorIndex, Direction facing,
								std::string name) {
	Entity entity = this->createActor(x, y, actorIndex, facing, name);

	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::PLAYER_ACTOR);

	HealthComponent healthCmpnt = mHealthSystem.getComponent(entity);
	mHealthSystem.setRespawn(healthCmpnt, true);

	GunComponent gunCmpnt = mGunSystem.getComponent(entity);
	mGunSystem.setBulletDamage(gunCmpnt, 10.0f);

	return entity;
}

//  ============================================================================
Entity World::createProp(glm::vec2 position, int tilesetId, float offsetZ) {
	return this->createProp(position, tilesetId, tilesetId, offsetZ);
}

//  ============================================================================
Entity World::createProp(glm::vec2 position, int frame1TilesetId,
						 int frame2TilesetId, float offsetZ) {
	Entity entity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setOffsetZ(spriteCmpnt, offsetZ);
	mSpriteSystem.setTileset(spriteCmpnt, "world");
	mSpriteSystem.setTilesetId(spriteCmpnt,
							   { frame1TilesetId, frame2TilesetId });

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, position + glm::vec2(8.0f, -8.0f));
	mPhysicsSystem.setRadius(physicsCmpnt, 0);

	return entity;
}

//  ============================================================================
Entity World::createTrigger(const float x, const float y, const float width,
						  const float height, const std::string targetName) {
	Entity entity = mEntityManager.createEntity();

	mTriggerSystem.addComponent(entity);
	TriggerComponent triggerCmpnt = mTriggerSystem.getComponent(entity);
	mTriggerSystem.setPosition(triggerCmpnt, glm::vec2(x, y));
	mTriggerSystem.setSize(triggerCmpnt, glm::vec2(width, height));
	mTriggerSystem.setTargetName(triggerCmpnt, targetName);

	return entity;
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
	mLightSystem.setGlowSize(lightCmpnt, 0);
	mLightSystem.setSize(lightCmpnt, 48);

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
	emitter.direction = glm::vec2(0.0f, 1.0f);
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

	if (mLightSystem.hasComponent(entity)) {
		mLightSystem.removeComponent(entity);
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
AiSystem& World::getAiSystem() {
	return mAiSystem;
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
LightSystem& World::getLightSystem() {
	return mLightSystem;
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
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
	mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);

	std::vector<PlayerStart> playerStarts = mMap->getPlayerStarts();

	assert(playerStarts.size() == 4);
	assert(mPlayers.size() == 4);

	for (size_t p = 0; p < mPlayers.size(); ++p) {
		PlayerStart& playerStart = playerStarts[p];

		Entity entity = this->createPlayerActor(
			playerStart.position.x,
			playerStart.position.y,
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
}
}