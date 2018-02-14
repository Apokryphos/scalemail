#include "bullet_util.hpp"
#include "gun_system.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace ScaleMail
{
//	============================================================================
static GunComponent makeComponent(const int index) {
	return GunComponent(index);
}

//	============================================================================
GunSystem::GunSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mGunData.reserve(maxComponents);
	mBulletData.reserve(maxComponents);
	mLightData.reserve(maxComponents);
}

//	============================================================================
void GunSystem::createComponent() {
	GunComponentData data = {};
	data.cooldownDuration = 0.05f;
	mGunData.push_back(data);

	BulletData bulletData = {};
	bulletData.damage = 1.0f;
	bulletData.speed = 128.0f;
	bulletData.impactTilesetId = getBulletImpactTilesetId(0);
	bulletData.tilesetId = getBulletTilesetId(0);
	mBulletData.push_back(bulletData);

	LightData lightData = {};
	lightData.color = getBulletLightColor(0);
	mLightData.push_back(lightData);
}

//	============================================================================
void GunSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mGunData, index);
	swapWithLastElementAndRemove(mBulletData, index);
	swapWithLastElementAndRemove(mLightData, index);
}

//	============================================================================
GunComponent GunSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
void GunSystem::setBulletDamage(const GunComponent& cmpnt,
										 float damage) {
	mBulletData[cmpnt.index].damage = damage;
}

//	============================================================================
void GunSystem::setBulletImpactTilesetId(const GunComponent& cmpnt,
										 int tilesetId) {
	mBulletData[cmpnt.index].impactTilesetId = tilesetId;
}

//	============================================================================
void GunSystem::setBulletLightColor(const GunComponent& cmpnt,
							  const glm::vec4 lightColor) {
	mLightData[cmpnt.index].color = lightColor;
}

//	============================================================================
void GunSystem::setBulletSpeed(const GunComponent& cmpnt, float speed) {
	mBulletData[cmpnt.index].speed = speed;
}

//	============================================================================
void GunSystem::setBulletTilesetId(const GunComponent& cmpnt, int tilesetId) {
	mBulletData[cmpnt.index].tilesetId = tilesetId;
}

//	============================================================================
void GunSystem::setCooldownDuration(const GunComponent& cmpnt, float duration) {
	mGunData[cmpnt.index].cooldownDuration = std::max(duration, 0.0f);
}

//	============================================================================
void GunSystem::setFire(const GunComponent& cmpnt, const bool fire) {
	mGunData[cmpnt.index].fire = fire;
}

//	============================================================================
void GunSystem::setPosition(const GunComponent& cmpnt,
							const glm::vec2 position) {
	mGunData[cmpnt.index].position = position;
}

//	============================================================================
void GunSystem::setTarget(const GunComponent& cmpnt, const glm::vec2 target) {
	mGunData[cmpnt.index].target = target;
}

//	============================================================================
void GunSystem::update(World& world, float elapsedSeconds) {
	//	Update position
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(p.second);

		mGunData[index].position = physicsSystem.getPosition(physicsCmpnt);

		glm::vec2 direction = mGunData[index].target - mGunData[index].position;

		if (glm::length2(direction) > 1) {
			direction = glm::normalize(direction);
		}

		mGunData[index].direction = direction;
	}

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		GunComponentData& gunData = mGunData[index];

		gunData.cooldownTicks =
			std::max(gunData.cooldownTicks - elapsedSeconds, 0.0f);

		if (gunData.fire && gunData.cooldownTicks <= 0.0f) {
			gunData.cooldownTicks = gunData.cooldownDuration;

			BulletData& bulletData = mBulletData[index];
			LightData& lightData = mLightData[index];

			world.createBullet(
				p.second,
				gunData.position,
				gunData.direction,
				bulletData.damage,
				bulletData.speed,
				bulletData.tilesetId,
				bulletData.impactTilesetId,
				lightData.color);
		}
	}
}
}