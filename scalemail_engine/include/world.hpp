#pragma once

#include "ai/ai_behavior_factory.hpp"
#include "ai_system.hpp"
#include "bullet_system.hpp"
#include "bury_system.hpp"
#include "damage_system.hpp"
#include "direction.hpp"
#include "door_system.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "expire_system.hpp"
#include "facing_system.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "light_system.hpp"
#include "loot_system.hpp"
#include "name_system.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "prefab_factory.hpp"
#include "random.hpp"
#include "sprite_system.hpp"
#include "sprite_effect_system.hpp"
#include "team_system.hpp"
#include "trigger_system.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ScaleMail
{
class Map;

class World
{
	EntityManager mEntityManager;
	std::shared_ptr<Map> mMap;

	Random mRandom;
	AiBehaviorFactory mAiBehaviorFactory;
	PrefabFactory mPrefabFactory;

	AiSystem mAiSystem;
	BulletSystem mBulletSystem;
	BurySystem mBurySystem;
	DamageSystem mDamageSystem;
	DoorSystem mDoorSystem;
	ExpireSystem mExpireSystem;
	FacingSystem mFacingSystem;
	GunSystem mGunSystem;
	HealthSystem mHealthSystem;
	InventorySystem mInventorySystem;
	LightSystem mLightSystem;
	LootSystem mLootSystem;
	NameSystem mNameSystem;
	ParticleSystem mParticleSystem;
	PhysicsSystem mPhysicsSystem;
	SpriteEffectSystem mSpriteEffectSystem;
	SpriteSystem mSpriteSystem;
	TeamSystem mTeamSystem;
	TriggerSystem mTriggerSystem;

	std::vector<Player> mPlayers;

public:
	World();
	Entity createEntity();
	void destroyEntity(Entity entity);
	bool entityIsAlive(const Entity& entity);
	AiBehaviorFactory& getAiBehaviorFactory();
	AiSystem& getAiSystem();
	BulletSystem& getBulletSystem();
	BurySystem& getBurySystem();
	DamageSystem& getDamageSystem();
	DoorSystem& getDoorSystem();
	ExpireSystem& getExpireSystem();
	FacingSystem& getFacingSystem();
	GunSystem& getGunSystem();
	HealthSystem& getHealthSystem();
	InventorySystem& getInventorySystem();
	LightSystem& getLightSystem();
	LootSystem& getLootSystem();
	Map* getMap();
	NameSystem& getNameSystem();
	ParticleSystem& getParticleSystem();
	PhysicsSystem& getPhysicsSystem();
	std::vector<Player*> getPlayers();
	PrefabFactory& getPrefabFactory();
	Random& getRandom();
	SpriteSystem& getSpriteSystem();
	SpriteEffectSystem& getSpriteEffectSystem();
	TeamSystem& getTeamSystem();
	TriggerSystem& getTriggerSystem();
	void initialize(AssetManager* assetManager);
	void loadMap(const std::string& mapName);
	void update(double totalElapsedSeconds, float elapsedSeconds);
};
}