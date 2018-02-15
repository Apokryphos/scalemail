#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class DamageSystem;
struct EntityCollision;
struct StaticCollision;
class World;

struct BulletComponent {
	BulletComponent(const int index) { this->index = index; }
	int index;
};

class BulletSystem : public EntitySystem
{
	std::vector<float> mLife;
	std::vector<float> mDamage;
	std::vector<int> mImpactTilesetId;
	std::vector<Entity> mSourceEntity;
	DamageSystem* mDamageSystem;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	BulletSystem(EntityManager& entityManager, int maxComponents = 1000);
	BulletComponent getComponent(const Entity& entity) const;
	int getImpactTilesetId(const BulletComponent& cmpnt) const;
	float getLife(const BulletComponent& cmpnt) const;
	Entity getSourceEntity(const BulletComponent& cmpnt) const;
	void initialize(DamageSystem& damageSystem);
	void onEntityCollision(EntityCollision& collision);
	void onStaticCollision(StaticCollision& collision);
	void setDamage(const BulletComponent& cmpnt, const float damage);
	void setImpactTilesetId(const BulletComponent& cmpnt, const int impactTilesetId);
	void setLife(const BulletComponent& cmpnt, const float life);
	void setSourceEntity(const BulletComponent& cmpnt, Entity entity);
	void simulate(World& world, float elapsedSeconds);
};
}