#pragma once

#include "entity_system.hpp"
#include "entity_collision.hpp"
#include "static_collision.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class World;

struct BulletComponent {
	BulletComponent(const int index) { this->index = index; }
	int index;
};

class BulletSystem : public EntitySystem
{
	std::vector<float> mLife;
	std::vector<Entity> mSourceEntity;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	BulletSystem(EntityManager& entityManager, int maxComponents = 1000);
	BulletComponent getComponent(const Entity& entity) const;
	float getLife(const BulletComponent& cmpnt) const;
	Entity getSourceEntity(const BulletComponent& cmpnt) const;
	void onEntityCollision(EntityCollision& collision);
	void onStaticCollision(StaticCollision& collision);
	void setLife(const BulletComponent& cmpnt, const float life);
	void setSourceEntity(const BulletComponent& cmpnt, Entity entity);
	void simulate(World& world, float elapsedSeconds);
};
}