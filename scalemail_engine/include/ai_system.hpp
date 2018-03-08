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
		bool seekEnabled;
		bool wanderEnabled;
		float wanderAngle;
		glm::vec2 avoidForce;
		glm::vec2 moveDirection;
		glm::vec2 position;
		glm::vec2 seekForce;
		glm::vec2 seekTarget;
		glm::vec2 wanderForce;
		std::vector<std::shared_ptr<AiBehavior>> behaviors;
	};

	bool mEnabled;

	std::vector<Obstacle> mObstacles;
	std::vector<AiComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	AiSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addBehavior(const AiComponent& cmpnt,
					 std::shared_ptr<AiBehavior> behavior);
	void addObstacle(const glm::vec2& position, const float radius);
	void addObstacle(const float x, const float y, const float width,
					 const float height);
	void drawDebug(std::vector<float>& lineVertexData);
	void enable(bool enabled);
	AiComponent getComponent(const Entity& entity) const;
	void setMoveDirection(const AiComponent& cmpnt, glm::vec2 direction);
	void setSeek(const AiComponent& cmpnt, bool enabled);
	void setSeekTarget(const AiComponent& cmpnt, const glm::vec2& target);
	void setWander(const AiComponent& cmpnt, bool enabled);
	void simulate(World& world);
	void update(World& world, double totalElapsedSeconds);
};
}