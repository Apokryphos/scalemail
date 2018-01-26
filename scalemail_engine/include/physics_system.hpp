#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
struct PhysicsComponent {
	PhysicsComponent(const int index) { this->index = index; }
	int index;
};

class PhysicsSystem : public EntitySystem
{
    std::vector<glm::vec2> mPosition;

	virtual void createComponent(const Entity& entity) override;
	virtual void destroyComponent(int index) override;

public:
	PhysicsSystem(EntityManager& entityManager, int maxComponents = 1000);
    PhysicsComponent getComponent(const Entity& entity) const;
    glm::vec2 getPosition(const PhysicsComponent& cmpnt) const;
  	void setPosition(const PhysicsComponent& cmpnt, const glm::vec2 position);
    void simulate(float elapsedSeconds);
};
}