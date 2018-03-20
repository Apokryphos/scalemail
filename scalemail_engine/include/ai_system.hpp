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
		bool avoidEnabled;
		bool seekEnabled;
		bool wanderEnabled;
		float arrivalRadius;
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

	std::vector<Obstacle> mStaticObstacles;
	std::vector<Obstacle> mStaticActorObstacles;
	std::vector<AiComponentData> mData;

	void addObstacle(const glm::vec2& position, const float radius,
					 bool actorObstacle);
	void addObstacle(const float x, const float y, const float width,
					 const float height, bool actorObstacle);
	glm::vec2 calculateAvoidForce(std::vector<Obstacle>& obstacles,
								  glm::vec2 position, glm::vec2 velocity);
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	AiSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addStaticActorObstacle(const float x, const float y,
								const float width, const float height);
	void addStaticObstacle(const float x, const float y, const float width,
					 	   const float height);
	void addBehavior(const AiComponent& cmpnt,
					 std::shared_ptr<AiBehavior> behavior);
	void drawDebug(std::vector<float>& lineVertexData);
	void enable(bool enabled);
	AiComponent getComponent(const Entity& entity) const;
	void setAvoid(const AiComponent& cmpnt, bool enabled);
	void setMoveDirection(const AiComponent& cmpnt, glm::vec2 direction);
	void setSeek(const AiComponent& cmpnt, bool enabled);
	void setSeekTarget(const AiComponent& cmpnt, const glm::vec2& target);
	void setWander(const AiComponent& cmpnt, bool enabled);
	void simulate(World& world);
	void update(World& world, double totalElapsedSeconds);
};
}