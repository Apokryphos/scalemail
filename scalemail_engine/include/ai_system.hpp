#pragma once

#include "entity_system.hpp"
#include <memory>
#include <vector>

namespace ScaleMail
{
class AiBehavior;
class World;

struct AiComponent {
	AiComponent(const int index) { this->index = index; }
	int index;
};

class AiSystem : public EntitySystem
{
	struct AiComponentData
	{
		std::vector<std::shared_ptr<AiBehavior>> behaviors;
	};

	std::vector<AiComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	AiSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addBehavior(const AiComponent& cmpnt,
					 std::shared_ptr<AiBehavior> behavior);
	AiComponent getComponent(const Entity& entity) const;
	void update(World& world, float elapsedSeconds);
};
}