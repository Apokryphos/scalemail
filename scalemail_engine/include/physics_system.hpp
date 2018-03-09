#pragma once

#include "entity_system.hpp"
#include "entity_collision.hpp"
#include "static_collision.hpp"
#include "trigger.hpp"
#include "trigger_collision.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <functional>
#include <vector>

namespace ScaleMail
{
struct PhysicsComponent {
	PhysicsComponent(const int index) { this->index = index; }
	int index;
};

class PhysicsSystem : public EntitySystem
{
	std::vector<bool> mIgnoreActorCollisions;
	std::vector<CollisionGroup> mGroup;
	std::vector<glm::vec2> mDirection;
	std::vector<glm::vec2> mPosition;
	std::vector<glm::vec2> mForce;
	std::vector<glm::vec2> mVelocity;
	std::vector<float> mRadius;
	std::vector<float> mSpeed;

	std::vector<glm::vec4> mStaticObstacles;
	std::vector<glm::vec4> mStaticActorObstacles;

	std::vector<EntityCollision> mEntityCollisions;
	std::vector<StaticCollision> mStaticCollisions;

	std::vector<std::function<void(EntityCollision&)>> mEntityCollisionCallbacks;
	std::vector<std::function<void(StaticCollision&)>> mStaticCollisionCallbacks;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	PhysicsSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addEntityCollisionCallback(std::function<void(EntityCollision&)> callback);
	void addForce(const PhysicsComponent& cmpnt, const glm::vec2 force);
	void addStaticCollisionCallback(std::function<void(StaticCollision&)> callback);
	void addStaticActorObstacle(const float x,     const float y,
							    const float width, const float height);
	void addStaticObstacle(const float x,     const float y,
						   const float width, const float height);
	void clearStaticObstacles();
	void drawDebug(std::vector<float>& lineVertexData);
	CollisionGroup getCollisionGroup(const PhysicsComponent& cmpnt) const;
	PhysicsComponent getComponent(const Entity& entity) const;
	glm::vec2 getDirection(const PhysicsComponent& cmpnt) const;
	std::vector<TriggerCollision> getEntityIntersections(
		const std::vector<Trigger>& triggers) const;
	bool getIgnoreActorCollisions(const PhysicsComponent& cmpnt) const;
	glm::vec2 getPosition(const PhysicsComponent& cmpnt) const;
	float getRadius(const PhysicsComponent& cmpnt) const;
	float getSpeed(const PhysicsComponent& cmpnt) const;
	glm::vec2 getVelocity(const PhysicsComponent& cmpnt) const;
	void setCollisionGroup(const PhysicsComponent& cmpnt,
						   const CollisionGroup group);
	void setDirection(const PhysicsComponent& cmpnt, const glm::vec2 direction);
  	void setIgnoreActorCollisions(const PhysicsComponent& cmpnt,
	  							  const bool ignore);
  	void setPosition(const PhysicsComponent& cmpnt, const glm::vec2 position);
	void setRadius(const PhysicsComponent& cmpnt, const float radius);
	void setSpeed(const PhysicsComponent& cmpnt, const float speed);
	void simulate(float elapsedSeconds);
	void update();
};
}