#pragma once

#include "entity_system.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class World;

struct GunComponent {
	GunComponent(const int index) { this->index = index; }
	int index;
};

class GunSystem : public EntitySystem
{
	struct GunComponentData
	{
		bool fire;
		int bulletTilesetId;
		float bulletSpeed;
		float cooldownDuration;
		float cooldownTicks;
		glm::vec2 direction;
		glm::vec2 position;
		glm::vec2 target;
	};

	std::vector<GunComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	GunSystem(EntityManager& entityManager, int maxComponents = 1000);
	GunComponent getComponent(const Entity& entity) const;
  	void setFire(const GunComponent& cmpnt, const bool fire);
  	void setPosition(const GunComponent& cmpnt, const glm::vec2 position);
  	void setTarget(const GunComponent& cmpnt, glm::vec2 target);
	void update(World& world, float elapsedSeconds);
};
}