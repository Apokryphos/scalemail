#pragma once

#include "ai_behavior_factory.hpp"
#include "ai_system.hpp"
#include "bullet_system.hpp"
#include "bury_system.hpp"
#include "damage_system.hpp"
#include "direction.hpp"
#include "door_system.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "expire_system.hpp"
#include "gun_system.hpp"
#include "health_system.hpp"
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

	PhysicsSystem mPhysicsSystem;
	SpriteSystem mSpriteSystem;
	LightSystem mLightSystem;
	NameSystem mNameSystem;
	BulletSystem mBulletSystem;
	ExpireSystem mExpireSystem;
	TriggerSystem mTriggerSystem;
	GunSystem mGunSystem;
	DoorSystem mDoorSystem;
	AiSystem mAiSystem;
	HealthSystem mHealthSystem;
	DamageSystem mDamageSystem;
	SpriteEffectSystem mSpriteEffectSystem;
	BurySystem mBurySystem;
	ParticleSystem mParticleSystem;
	LootSystem mLootSystem;

	std::vector<Player> mPlayers;

public:
	World();
	Entity createActor(float x, float y, glm::vec2 size, int actorIndex,
					   Direction facing = Direction::SOUTH,
					   const std::string name = "",
					   const std::string prefab = "",
					   const std::string ai = "");
	Entity createBullet(Entity sourceEntity, glm::vec2 position,
						glm::vec2 direction, float damage, float speed,
						int tilesetId, int impactTilesetId,
						glm::vec4 lightColor);
	Entity createDoor(float x, float y, int openTilesetId, int closedTilesetId,
					  bool open, const std::string name = "");
	Entity createDoor(float x, float y, int openTilesetId,
					  int closedTilesetId, int childOpenTilesetId,
					  int childClosedTilesetId, bool open,
					  const std::string name);
	Entity createEntity();
	Entity createLoot(glm::vec2 position, glm::vec2 size, int tilesetId,
					  std::string name, std::string prefab);
	Entity createPlayerActor(float x, float y, int actorIndex,
							 Direction facing = Direction::SOUTH,
							 const std::string name = "");
	Entity createProp(glm::vec2 position, int tilesetId, bool decal);
	Entity createProp(glm::vec2 position, int frame1TilesetId,
					  int frame2TilesetId, bool decal);
	Entity createTrigger(const float x, const float y, const float width,
						 const float height, const std::string targetName);
	void destroyBullet(Entity entity);
	void destroyEntity(Entity entity);
	bool entityIsAlive(const Entity& entity);
	AiSystem& getAiSystem();
	BurySystem& getBurySystem();
	DamageSystem& getDamageSystem();
	DoorSystem& getDoorSystem();
	std::vector<Entity> getEntitiesByName(const std::string name) const;
	GunSystem& getGunSystem();
	HealthSystem& getHealthSystem();
	LightSystem& getLightSystem();
	LootSystem& getLootSystem();
	Map* getMap();
	NameSystem& getNameSystem();
	ParticleSystem& getParticleSystem();
	PhysicsSystem& getPhysicsSystem();
	std::vector<Player*> getPlayers();
	Random& getRandom();
	SpriteSystem& getSpriteSystem();
	SpriteEffectSystem& getSpriteEffectSystem();
	TriggerSystem& getTriggerSystem();
	void initialize(AssetManager* assetManager);
	void loadMap(const std::string& mapName);
	void update(float elapsedSeconds);
};
}