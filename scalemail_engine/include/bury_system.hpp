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
class SpriteSystem;

enum class BuryState
{
	NORMAL,
	BURYING,
	BURIED,
	RISING,
};

struct BuryComponent {
	BuryComponent(const int index) { this->index = index; }
	int index;
};

struct BuryComponentData {
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
	std::vector<BuryComponentData> mData;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	BurySystem(EntityManager& entityManager, int maxComponents = 10000);
	BuryComponent getComponent(const Entity& entity) const;
	void bury(const BuryComponent& cmpnt, bool immediate);
	void initialize(PhysicsSystem& physicsSystem, SpriteSystem& spriteSystem);
	void rise(const BuryComponent& cmpnt, bool immediate);
	void setDuration(const BuryComponent& cmpnt, const float duration);
	void update(float elapsedSeconds);
};
}