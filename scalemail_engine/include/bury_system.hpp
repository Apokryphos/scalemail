#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
struct Entity;
class EntityManager;
class PhysicsSystem;
class Random;
class SpriteSystem;

enum class BuryState
{
	NORMAL,
	BURYING,
	BURIED,
	RISING,
};

std::string buryStateToString(BuryState buryState);

struct BuryComponent {
	BuryComponent(const int index) { this->index = index; }
	int index;
};

struct BuryComponentData {
	bool holeSpawned;
	bool spawnDirt;
	BuryState buryState;
	float ticks;
	float duration;
	float offsetY;
	float collisionRadius;
	glm::vec2 size;
	glm::ivec4 sourceRect;
};

class BurySystem : public EntitySystem
{
private:
	PhysicsSystem* mPhysicsSystem;
	SpriteSystem* mSpriteSystem;
	Random* mRandom;

	std::vector<BuryComponentData> mData;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;
	void spawnDirt(const glm::vec2 origin, const float progress);
	void spawnHole(const glm::vec2 origin);

public:
	BurySystem(EntityManager& entityManager, int maxComponents = 10000);
	BuryComponent getComponent(const Entity& entity) const;
	void bury(const BuryComponent& cmpnt, bool immediate);
	BuryState getBuryState(const BuryComponent& cmpnt) const;
	void initialize(Random& random, PhysicsSystem& physicsSystem,
					SpriteSystem& spriteSystem);
	void rise(const BuryComponent& cmpnt, bool immediate);
	void setDuration(const BuryComponent& cmpnt, const float duration);
	void setSpawnDirt(const BuryComponent& cmpnt, bool spawn);
	void update(float elapsedSeconds);
};
}