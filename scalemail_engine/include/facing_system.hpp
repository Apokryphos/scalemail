#pragma once

#include "entity_system.hpp"

namespace ScaleMail
{
struct Entity;
class EntityManager;
class PhysicsSystem;
class SpriteSystem;

struct FacingComponent {
	FacingComponent(const int index) { this->index = index; }
	int index;
};

class FacingSystem : public EntitySystem
{
protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	FacingSystem(EntityManager& entityManager, int maxComponents = 10000);
	FacingComponent getComponent(const Entity& entity) const;
	void update(PhysicsSystem& physicsSystem, SpriteSystem& spriteSystem);
};
}