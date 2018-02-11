#pragma once

#include "ai_system.hpp"
#include "bullet_system.hpp"
#include "direction.hpp"
#include "door_system.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "expire_system.hpp"
#include "gun_system.hpp"
#include "light_system.hpp"
#include "map.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "sprite_system.hpp"
#include "trigger_system.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ScaleMail
{
class World
{
	EntityManager mEntityManager;
	std::shared_ptr<Map> mMap;

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

	std::vector<Player> mPlayers;

public:
	World();
	Entity createActor(float x, float y, int actorIndex,
					   Direction facing = Direction::SOUTH,
					   const std::string name = "");
	Entity createBullet(glm::vec2 position, glm::vec2 direction, float speed,
						int tilesetId);
	Entity createDoor(float x, float y, int openTilesetId, int closedTilesetId,
					  bool open, const std::string name = "");
	Entity createDoor(float x, float y, int openTilesetId,
					  int closedTilesetId, int childOpenTilesetId,
					  int childClosedTilesetId, bool open,
					  const std::string name);
	Entity createEntity();
	Entity createPlayerActor(float x, float y, int actorIndex,
							 Direction facing = Direction::SOUTH,
							 const std::string name = "");
	Entity createPlayerBullet(glm::vec2 position, glm::vec2 direction,
							  float speed, int tilesetId);
	Entity createProp(glm::vec2 position, int tilesetId, float offsetZ);
	Entity createProp(glm::vec2 position, int frame1TilesetId,
					  int frame2TilesetId, float offsetZ);
	Entity createTrigger(const float x, const float y, const float width,
						 const float height, const std::string targetName);
	void destroyBullet(Entity entity);
	void destroyEntity(Entity entity);
	AiSystem& getAiSystem();
	DoorSystem& getDoorSystem();
	std::vector<Entity> getEntitiesByName(const std::string name) const;
	GunSystem& getGunSystem();
	LightSystem& getLightSystem();
	Map* getMap();
	NameSystem& getNameSystem();
	PhysicsSystem& getPhysicsSystem();
	std::vector<Player*> getPlayers();
	SpriteSystem& getSpriteSystem();
	TriggerSystem& getTriggerSystem();
	void initialize(AssetManager* assetManager);
	void loadMap(const std::string& mapName);
	void update(float elapsedSeconds);
};
}