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
	mData.reserve(maxComponents);
}

//	============================================================================
void GunSystem::createComponent() {
	GunComponentData data = {};
	data.cooldownDuration = 0.05f;
	data.bulletSpeed = 128.0f;
	data.bulletTilesetId = 32;
	mData.push_back(data);
}

//	============================================================================
void GunSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
GunComponent GunSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
void GunSystem::setFire(const GunComponent& cmpnt, const bool fire) {
	mData[cmpnt.index].fire = fire;
}

//	============================================================================
void GunSystem::setPosition(const GunComponent& cmpnt,
							const glm::vec2 position) {
	mData[cmpnt.index].position = position;
}

//	============================================================================
void GunSystem::setTarget(const GunComponent& cmpnt, const glm::vec2 target) {
	mData[cmpnt.index].target = target;
}

//	============================================================================
void GunSystem::update(World& world, float elapsedSeconds) {
	//	Update position
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(p.second);

		mData[index].position = physicsSystem.getPosition(physicsCmpnt);

		glm::vec2 direction = mData[index].target - mData[index].position;

		if (glm::length2(direction) > 1) {
			direction = glm::normalize(direction);
		}

		mData[index].direction = direction;
	}

	size_t count = mData.size();
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		GunComponentData& data = mData[index];

		data.cooldownTicks = std::max(data.cooldownTicks - elapsedSeconds, 0.0f);

		if (data.fire && data.cooldownTicks <= 0.0f) {
			world.createBullet(
				p.second,
				data.position,
				data.direction,
				data.bulletSpeed,
				data.bulletTilesetId);

			data.cooldownTicks = data.cooldownDuration;
		}
	}
}
}