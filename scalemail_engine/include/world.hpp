#pragma once

#include "entity.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ScaleMail
{
class AiBehaviorFactory;
class AiSystem;
class AssetManager;
class BulletSystem;
class BurySystem;
class DamageSystem;
class DoorSystem;
class ExpireSystem;
class FacingSystem;
class GunSystem;
class HealthSystem;
class InventorySystem;
class LightSystem;
class LootSystem;
class Map;
class NameSystem;
class ParticleSystem;
class PhysicsSystem;
class Player;
class PrefabFactory;
class Random;
class SpriteSystem;
class SpriteEffectSystem;
class TeamSystem;
class TriggerSystem;

class World
{
private:
	class WorldImpl;
	class WorldSystems;

	AssetManager* mAssetManager;

	std::unique_ptr<WorldImpl> mImpl;
	std::unique_ptr<WorldSystems> mSystems;
	std::shared_ptr<Map> mMap;

public:
	World();
	~World();
	World(const World&) = delete;
	World& operator=(const World&) = delete;
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
	void initialize(AssetManager& assetManager);
	void loadMap(const std::string& mapName);
	void update(double totalElapsedSeconds, float elapsedSeconds);
};
}