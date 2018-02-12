#pragma once

#include "entity_system.hpp"
#include <vector>

namespace ScaleMail
{
class HealthSystem;

struct DamageComponent {
	DamageComponent(const int index) { this->index = index; }
	int index;
};

class DamageSystem : public EntitySystem
{
	struct DamageComponentData
	{
		float damage;
	};

	std::vector<DamageComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	DamageSystem(EntityManager& entityManager, int maxComponents = 1000);
	void applyDamage(HealthSystem& healthSystem);
	DamageComponent getComponent(const Entity& entity) const;
	float getDamage(const DamageComponent& cmpnt) const;
};
}