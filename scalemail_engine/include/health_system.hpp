#pragma once

#include "entity_system.hpp"
#include "health_gauge.hpp"
#include <vector>

namespace ScaleMail
{
class World;

struct HealthComponent {
	HealthComponent(const int index) { this->index = index; }
	int index;
};

class HealthSystem : public EntitySystem
{
	struct HealthComponentData
	{
		bool respawn;
		HealthGauge healthGauge;
	};

	std::vector<HealthComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	HealthSystem(EntityManager& entityManager, int maxComponents = 1000);
	HealthComponent getComponent(const Entity& entity) const;
	HealthGauge& getHealthGauge(const HealthComponent& cmpnt);
	void setRespawn(const HealthComponent& cmpnt, const bool respawn);
	void update(World& world);
};
}