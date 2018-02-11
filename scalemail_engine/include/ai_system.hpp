#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class World;

struct AiComponent {
	AiComponent(const int index) { this->index = index; }
	int index;
};

class AiSystem : public EntitySystem
{
	struct AiComponentData
	{
	};

	std::vector<AiComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	AiSystem(EntityManager& entityManager, int maxComponents = 1000);
	AiComponent getComponent(const Entity& entity) const;
	void update(World& world, float elapsedSeconds);
};
}