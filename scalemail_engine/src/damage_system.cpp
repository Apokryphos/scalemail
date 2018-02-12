#include "damage_system.hpp"
#include "health_system.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
//	============================================================================
static DamageComponent makeComponent(const int index) {
	return DamageComponent(index);
}

//	============================================================================
DamageSystem::DamageSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void DamageSystem::applyDamage(HealthSystem& healthSystem) {
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;
		const Entity& entity = p.second;

		float damage = mData[index].damage;

		if (damage > 0.0f && healthSystem.hasComponent(entity)) {
			HealthComponent healthCmpnt = healthSystem.getComponent(entity);
			HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
			healthGauge.subtract(damage);
			mData[index].damage = 0.0f;
		}
	}
}

//	============================================================================
void DamageSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void DamageSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
DamageComponent DamageSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
float DamageSystem::getDamage(const DamageComponent& cmpnt) const {
	return mData[cmpnt.index].damage;
}
}