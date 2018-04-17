#pragma once

#include "bullet_data.hpp"
#include "entity_system.hpp"
#include "light_data.hpp"
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
		float cooldownDuration;
		float cooldownTicks;
		glm::vec2 direction;
		glm::vec2 position;
		glm::vec2 target;
	};

	std::vector<GunComponentData> mGunData;
	std::vector<BulletData> mBulletData;
	std::vector<LightData> mLightData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	GunSystem(EntityManager& entityManager, int maxComponents = 1000);
	GunComponent getComponent(const Entity& entity) const;
	BulletData getBulletData(const GunComponent& cmpnt) const;
	LightData getLightData(const GunComponent& cmpnt) const;
	void setBulletDamage(const GunComponent& cmpnt, float damage);
	void setBulletData(const GunComponent& cmpnt, const BulletData& bulletData);
	void setBulletImpactTilesetId(const GunComponent& cmpnt, int tilesetId);
	void setBulletLightColor(const GunComponent& cmpnt,
							 const glm::vec4 lightColor);
	void setBulletRotateOffset(const GunComponent& cmpnt, float offset);
	void setBulletSpeed(const GunComponent& cmpnt, float speed);
	void setBulletTilesetId(const GunComponent& cmpnt, int tilesetId);
	void setCooldownDuration(const GunComponent& cmpnt, float duration);
  	void setFire(const GunComponent& cmpnt, const bool fire);
	void setLightData(const GunComponent& cmpnt, const LightData& lightData);
  	void setPosition(const GunComponent& cmpnt, const glm::vec2 position);
  	void setTarget(const GunComponent& cmpnt, const glm::vec2 target);
	void update(World& world, float elapsedSeconds);
};
}