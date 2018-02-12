#include "health_system.hpp"
#include "vector_util.hpp"

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
}