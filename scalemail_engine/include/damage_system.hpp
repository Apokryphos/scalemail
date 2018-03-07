#pragma once

#include "entity_system.hpp"
#include <unordered_set>
#include <vector>

namespace ScaleMail
{
class HealthSystem;
class SpriteEffectSystem;

struct DamageComponent {
	DamageComponent(const int index) { this->index = index; }
	int index;
};

class DamageSystem : public EntitySystem
{
	struct DamageComponentData
	{
		float damage;
		std::unordered_set<Entity> sourceEntities;
	};

	std::vector<DamageComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	DamageSystem(EntityManager& entityManager, int maxComponents = 1000);
	void addDamage(const DamageComponent& cmpnt, const float damage);
	void addSourceEntity(const DamageComponent& cmpnt, const Entity& source);
	void applyDamage(HealthSystem& healthSystem, SpriteEffectSystem& spriteEffectSystem);
	DamageComponent getComponent(const Entity& entity) const;
	float getDamage(const DamageComponent& cmpnt) const;
	const std::unordered_set<Entity>& getSourceEntities(const DamageComponent& cmpnt);
};
}