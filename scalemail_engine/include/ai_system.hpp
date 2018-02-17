#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
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
	struct Obstacle
	{
		float radius;
		glm::vec2 position;
	};

	struct AiComponentData
	{
		std::vector<std::shared_ptr<AiBehavior>> behaviors;
	};

	std::vector<Obstacle> mObstacles;
	std::vector<AiComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	AiSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addBehavior(const AiComponent& cmpnt,
					 std::shared_ptr<AiBehavior> behavior);
	void addObstacle(const glm::vec2& position, const float radius);
	AiComponent getComponent(const Entity& entity) const;
	void update(World& world, float elapsedSeconds);
};
}