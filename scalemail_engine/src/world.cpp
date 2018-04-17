#include "ai/ai_behavior_factory.hpp"
#include "ai_system.hpp"
#include "dev/debug_system.hpp"
#include "asset_manager.hpp"
#include "bullet_system.hpp"
#include "bullet_util.hpp"
#include "bury_system.hpp"
#include "camera_system.hpp"
#include "damage_system.hpp"
#include "direction.hpp"
#include "door_system.hpp"
#include "entity_manager.hpp"
#include "entity_types.hpp"
#include "entity_util.hpp"
#include "expire_system.hpp"
#include "facing_system.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "input.hpp"
#include "inventory_system.hpp"
#include "light_system.hpp"
#include "loot_system.hpp"
#include "map.hpp"
#include "map_load.hpp"
#include "math_util.hpp"
#include "name_system.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "prefab_factory.hpp"
#include "random.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include "sprite_effect_system.hpp"
#include "team_system.hpp"
#include "trigger_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
class World::WorldImpl
{
public:
	AiBehaviorFactory aiBehaviorFactory;
	std::vector<Player> players;
	PrefabFactory prefabFactory;
	Random random;
};

//  ============================================================================
class World::WorldSystems
{
public:
	WorldSystems(World& world)
	:	aiSystem(entityManager),
		bulletSystem(entityManager),
		burySystem(entityManager),
		cameraSystem(world, entityManager),
		damageSystem(entityManager),
		debugSystem(world, entityManager),
		doorSystem(entityManager),
		expireSystem(entityManager),
		facingSystem(entityManager),
		gunSystem(entityManager),
		healthSystem(entityManager),
		inventorySystem(world, entityManager),
		lightSystem(entityManager),
		lootSystem(entityManager),
		nameSystem(entityManager),
		particleSystem(entityManager),
		physicsSystem(entityManager, 10000),
		spriteSystem(entityManager),
		spriteEffectSystem(entityManager),
		teamSystem(entityManager),
		triggerSystem(entityManager) {
	}

	EntityManager entityManager;
	AiSystem aiSystem;
	BulletSystem bulletSystem;
	BurySystem burySystem;
	CameraSystem cameraSystem;
	DamageSystem damageSystem;
	DebugSystem debugSystem;
	DoorSystem doorSystem;
	ExpireSystem expireSystem;
	FacingSystem facingSystem;
	GunSystem gunSystem;
	HealthSystem healthSystem;
	InventorySystem inventorySystem;
	LightSystem lightSystem;
	LootSystem lootSystem;
	NameSystem nameSystem;
	ParticleSystem particleSystem;
	PhysicsSystem physicsSystem;
	SpriteSystem spriteSystem;
	SpriteEffectSystem spriteEffectSystem;
	TeamSystem teamSystem;
	TriggerSystem triggerSystem;
};

//  ============================================================================
World::World() : mImpl(std::make_unique<WorldImpl>()),
				 mSystems(std::make_unique<WorldSystems>(*this)) {
	mImpl->players.reserve(4);
	mImpl->players.emplace_back("Player1");
	mImpl->players.emplace_back("Player2");
	mImpl->players.emplace_back("Player3");
	mImpl->players.emplace_back("Player4");

	//	Assign keyboard to first player
	mImpl->players[0].inputDevice = &getKeyboardInputDevice();

	mSystems->physicsSystem.addEntityCollisionCallback(
		std::bind(&BulletSystem::onEntityCollision, &mSystems->bulletSystem,
				  std::placeholders::_1));

	mSystems->physicsSystem.addStaticCollisionCallback(
		std::bind(&BulletSystem::onStaticCollision, &mSystems->bulletSystem,
				  std::placeholders::_1));

	mSystems->physicsSystem.addEntityCollisionCallback(
		std::bind(&LootSystem::onEntityCollision, &mSystems->lootSystem,
				  std::placeholders::_1));
}

//  ============================================================================
World::~World() = default;

//  ============================================================================
Entity World::createEntity() {
	return mSystems->entityManager.createEntity();
}

//  ============================================================================
void World::destroyEntity(Entity entity) {
	if (mSystems->aiSystem.hasComponent(entity)) {
		mSystems->aiSystem.removeComponent(entity);
	}

	if (mSystems->bulletSystem.hasComponent(entity)) {
		mSystems->bulletSystem.removeComponent(entity);
	}

	if (mSystems->burySystem.hasComponent(entity)) {
		mSystems->burySystem.removeComponent(entity);
	}

	if (mSystems->cameraSystem.hasComponent(entity)) {
		mSystems->cameraSystem.removeComponent(entity);
	}

	if (mSystems->damageSystem.hasComponent(entity)) {
		mSystems->damageSystem.removeComponent(entity);
	}

	if (mSystems->debugSystem.hasComponent(entity)) {
		mSystems->debugSystem.removeComponent(entity);
	}

	if (mSystems->doorSystem.hasComponent(entity)) {
		mSystems->doorSystem.removeComponent(entity);
	}

	if (mSystems->expireSystem.hasComponent(entity)) {
		mSystems->expireSystem.removeComponent(entity);
	}

	if (mSystems->facingSystem.hasComponent(entity)) {
		mSystems->facingSystem.removeComponent(entity);
	}

	if (mSystems->gunSystem.hasComponent(entity)) {
		mSystems->gunSystem.removeComponent(entity);
	}

	if (mSystems->healthSystem.hasComponent(entity)) {
		mSystems->healthSystem.removeComponent(entity);
	}

	if (mSystems->inventorySystem.hasComponent(entity)) {
		mSystems->inventorySystem.removeComponent(entity);
	}

	if (mSystems->lightSystem.hasComponent(entity)) {
		mSystems->lightSystem.removeComponent(entity);
	}

	if (mSystems->lootSystem.hasComponent(entity)) {
		mSystems->lootSystem.removeComponent(entity);
	}

	if (mSystems->nameSystem.hasComponent(entity)) {
		mSystems->nameSystem.removeComponent(entity);
	}

	if (mSystems->particleSystem.hasComponent(entity)) {
		mSystems->particleSystem.removeComponent(entity);
	}

	if (mSystems->physicsSystem.hasComponent(entity)) {
		mSystems->physicsSystem.removeComponent(entity);
	}

	if (mSystems->spriteSystem.hasComponent(entity)) {
		mSystems->spriteSystem.removeComponent(entity);
	}

	if (mSystems->spriteEffectSystem.hasComponent(entity)) {
		mSystems->spriteEffectSystem.removeComponent(entity);
	}

	if (mSystems->teamSystem.hasComponent(entity)) {
		mSystems->teamSystem.removeComponent(entity);
	}

	if (mSystems->triggerSystem.hasComponent(entity)) {
		mSystems->triggerSystem.removeComponent(entity);
	}

	mSystems->entityManager.destroyEntity(entity);
}

//  ============================================================================
AiBehaviorFactory& World::getAiBehaviorFactory() {
	return mImpl->aiBehaviorFactory;
}

//  ============================================================================
AiSystem& World::getAiSystem() {
	return mSystems->aiSystem;
}

//  ============================================================================
BulletSystem& World::getBulletSystem() {
	return mSystems->bulletSystem;
}

//  ============================================================================
BurySystem& World::getBurySystem() {
	return mSystems->burySystem;
}

//  ============================================================================
CameraSystem& World::getCameraSystem() {
	return mSystems->cameraSystem;
}

//  ============================================================================
DamageSystem& World::getDamageSystem() {
	return mSystems->damageSystem;
}

//  ============================================================================
DebugSystem& World::getDebugSystem() {
	return mSystems->debugSystem;
}

//  ============================================================================
DoorSystem& World::getDoorSystem() {
	return mSystems->doorSystem;
}

//  ============================================================================
ExpireSystem& World::getExpireSystem() {
	return mSystems->expireSystem;
}

//  ============================================================================
FacingSystem& World::getFacingSystem() {
	return mSystems->facingSystem;
}

//  ============================================================================
GunSystem& World::getGunSystem() {
	return mSystems->gunSystem;
}

//  ============================================================================
HealthSystem& World::getHealthSystem() {
	return mSystems->healthSystem;
}

//  ============================================================================
InventorySystem& World::getInventorySystem() {
	return mSystems->inventorySystem;
}

//  ============================================================================
LightSystem& World::getLightSystem() {
	return mSystems->lightSystem;
}

//  ============================================================================
LootSystem& World::getLootSystem() {
	return mSystems->lootSystem;
}

//  ============================================================================
ParticleSystem& World::getParticleSystem() {
	return mSystems->particleSystem;
}

//  ============================================================================
std::vector<Player*> World::getPlayers() {
	std::vector<Player*> players;

	for (Player& player : mImpl->players) {
		players.push_back(&player);
	}

	return players;
}

//  ============================================================================
PrefabFactory& World::getPrefabFactory() {
	return mImpl->prefabFactory;
}

//  ============================================================================
Map* World::getMap() {
	return mMap.get();
}

//  ============================================================================
NameSystem& World::getNameSystem() {
	return mSystems->nameSystem;
}

//  ============================================================================
PhysicsSystem& World::getPhysicsSystem() {
	return mSystems->physicsSystem;
}

//  ============================================================================
Random& World::getRandom() {
	return mImpl->random;
}

//  ============================================================================
SpriteEffectSystem& World::getSpriteEffectSystem() {
	return mSystems->spriteEffectSystem;
}

//  ============================================================================
SpriteSystem& World::getSpriteSystem() {
	return mSystems->spriteSystem;
}

//  ============================================================================
TeamSystem& World::getTeamSystem() {
	return mSystems->teamSystem;
}

//  ============================================================================
TriggerSystem& World::getTriggerSystem() {
	return mSystems->triggerSystem;
}

//  ============================================================================
void World::initialize(AssetManager& assetManager) {
	mAssetManager = &assetManager;

	mSystems->spriteSystem.initialize(assetManager);
	mSystems->lightSystem.initialize(assetManager);
	mSystems->doorSystem.initialize(mSystems->physicsSystem,
									mSystems->spriteSystem);
	mSystems->bulletSystem.initialize(mSystems->damageSystem);
	mSystems->burySystem.initialize(mImpl->random, mSystems->physicsSystem,
									mSystems->spriteSystem);
	mSystems->particleSystem.initialize(assetManager, mImpl->random);
	mSystems->lootSystem.initialize(mSystems->inventorySystem);
}

//  ============================================================================
bool World::entityIsAlive(const Entity& entity) {
	return mSystems->entityManager.isAlive(entity);
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
	mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this,
							  *mAssetManager);

	std::vector<PlayerStart> playerStarts = mMap->getPlayerStarts();

	assert(playerStarts.size() == 4);
	assert(mImpl->players.size() == 4);

	for (size_t p = 0; p < mImpl->players.size(); ++p) {
		PlayerStart& playerStart = playerStarts[p];

		Entity entity = createPlayerActor(
			*this,
			playerStart.position,
			playerStart.actorIndex,
			playerStart.facing,
			"Player" + std::to_string(p + 1));

		mImpl->players[p].entity = entity;
		mImpl->players[p].actorIndex = playerStart.actorIndex;

		//	HACK: Add ally AI to other players for testing
		if (p > 0) {
			addAiBehavior("ally", entity, mSystems->aiSystem,
						  mImpl->aiBehaviorFactory);

			addPrefab("ally", entity, *this);
		}
	}
}

//  ============================================================================
void World::update(double totalElapsedSeconds, float elapsedSeconds) {
	mSystems->cameraSystem.update(elapsedSeconds);

	mSystems->aiSystem.update(*this, totalElapsedSeconds);

	mSystems->bulletSystem.simulate(*this, elapsedSeconds);

	mSystems->gunSystem.update(*this, elapsedSeconds);

	//	Simulate collisions
	mSystems->physicsSystem.simulate(elapsedSeconds);

	//	Update sprite facing using PhysicsSystem direction
	mSystems->facingSystem.update(mSystems->physicsSystem, mSystems->spriteSystem);

	//	Update positions
	mSystems->physicsSystem.update();

	mSystems->spriteSystem.update(elapsedSeconds, mSystems->physicsSystem);

	mSystems->lightSystem.update(elapsedSeconds, mSystems->physicsSystem);

	mSystems->triggerSystem.update(
		mSystems->nameSystem,
		mSystems->physicsSystem,
		mSystems->doorSystem);

	mSystems->damageSystem.applyDamage(mSystems->healthSystem, mSystems->spriteEffectSystem);

	mSystems->spriteEffectSystem.update(elapsedSeconds, mSystems->spriteSystem);

	mSystems->burySystem.update(elapsedSeconds);

	mSystems->healthSystem.update(*this);

	mSystems->lootSystem.simulate(*this, elapsedSeconds);

	mSystems->particleSystem.update(mSystems->physicsSystem, elapsedSeconds);

	//	Remove expired entities
	mSystems->expireSystem.update(*this, elapsedSeconds);
}
}