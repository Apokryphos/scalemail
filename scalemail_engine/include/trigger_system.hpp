#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class DoorSystem;
class NameSystem;
class PhysicsSystem;

struct TriggerComponent {
	TriggerComponent(const int index) { this->index = index; }
	int index;
};

class TriggerSystem : public EntitySystem
{
	struct TriggerComponentData
	{
		glm::vec2 position;
		glm::vec2 size;
		std::string targetName;
		std::vector<Entity> lastEntities;
		std::vector<Entity> entities;
	};

	std::vector<TriggerComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	TriggerSystem(EntityManager& entityManager, int maxComponents = 1000);
	TriggerComponent getComponent(const Entity& entity) const;
  	void setPosition(const TriggerComponent& cmpnt, const glm::vec2 position);
	void setSize(const TriggerComponent& cmpnt, const glm::vec2 size);
	void setTargetName(const TriggerComponent& cmpnt, const std::string name);
	void update(
		const NameSystem& nameSystem,
		const PhysicsSystem& physicsSystem,
		DoorSystem& doorSystem);
};
}