#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class World;

struct ExpireComponent {
	ExpireComponent(const int index) { this->index = index; }
	int index;
};

class ExpireSystem : public EntitySystem
{
	std::vector<float> mDuration;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	ExpireSystem(EntityManager& entityManager, int maxComponents = 1000);
	ExpireComponent getComponent(const Entity& entity) const;
	float getDuration(const ExpireComponent& cmpnt) const;
	void setDuration(const ExpireComponent& cmpnt, const float duration);
	void update(World& world, float elapsedSeconds);
};
}