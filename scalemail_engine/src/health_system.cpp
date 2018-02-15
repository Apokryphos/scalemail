#include "health_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
static HealthComponent makeComponent(const int index) {
	return HealthComponent(index);
}

//	============================================================================
HealthSystem::HealthSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void HealthSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void HealthSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
HealthComponent HealthSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
HealthGauge& HealthSystem::getHealthGauge(const HealthComponent& cmpnt) {
	return mData[cmpnt.index].healthGauge;
}

//	============================================================================
void HealthSystem::setRespawn(const HealthComponent& cmpnt, const bool respawn) {
	mData[cmpnt.index].respawn = respawn;
}

//	============================================================================
void HealthSystem::update(World& world) {
	std::vector<Entity> removeEntities;

	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		if (mData[index].healthGauge.isEmpty()) {
			//	Only destroy entities if they don't respawn
			if (!mData[index].respawn) {
				removeEntities.push_back(p.second);
			} else {
				//	Fill health gauge for now...
				mData[index].healthGauge.fill();
			}
		}
	}

	for (const auto& entity : removeEntities) {
		world.destroyEntity(entity);
	}
}
}