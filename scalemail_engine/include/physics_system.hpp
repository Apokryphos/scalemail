#pragma once

#include "entity_system.hpp"
#include "line_shader.hpp"
#include "mesh.hpp"
#include "static_collision.hpp"
#include <glm/vec2.hpp>
#include <functional>
#include <vector>

namespace ScaleMail
{
class AssetManager;
class Camera;

struct PhysicsComponent {
	PhysicsComponent(const int index) { this->index = index; }
	int index;
};

class PhysicsSystem : public EntitySystem
{
	std::vector<CollisionGroup> mGroup;
	std::vector<glm::vec2> mDirection;
	std::vector<glm::vec2> mPosition;
	std::vector<glm::vec2> mCollisionOffset;
	std::vector<float> mRadius;
	std::vector<float> mSpeed;

	std::vector<glm::vec4> mStaticObstacles;
	std::vector<glm::vec4> mStaticActorObstacles;

	std::vector<std::function<void(StaticCollision)>> mStaticCollisionCallbacks;

	Mesh mLineMesh;
	LineShader mLineShader;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	PhysicsSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addStaticCollisionCallback(std::function<void(StaticCollision)> callback);
	void addStaticActorObstacle(const float x,     const float y,
							    const float width, const float height);
	void addStaticObstacle(const float x,     const float y,
						   const float width, const float height);
	void clearStaticObstacles();
	void drawDebug(const Camera& camera);
	PhysicsComponent getComponent(const Entity& entity) const;
	glm::vec2 getPosition(const PhysicsComponent& cmpnt) const;
	void initialize(AssetManager& assetManager);
	void setCollisionGroup(const PhysicsComponent& cmpnt,
						   const CollisionGroup group);
	void setCollisionOffset(const PhysicsComponent& cmpnt, const glm::vec2 offset);
	void setDirection(const PhysicsComponent& cmpnt, const glm::vec2 direction);
  	void setPosition(const PhysicsComponent& cmpnt, const glm::vec2 position);
	void setRadius(const PhysicsComponent& cmpnt, const float radius);
	void setSpeed(const PhysicsComponent& cmpnt, const float speed);
	void simulate(float elapsedSeconds);
};
}