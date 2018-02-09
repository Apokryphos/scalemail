#include "expire_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <cmath>

namespace ScaleMail
{
//	============================================================================
static ExpireComponent makeComponent(const int index) {
	return ExpireComponent(index);
}

//	============================================================================
ExpireSystem::ExpireSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mDuration.reserve(maxComponents);
}

//	============================================================================
void ExpireSystem::createComponent() {
	mDuration.emplace_back(0);
}

//	============================================================================
void ExpireSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mDuration, index);
}

//	============================================================================
ExpireComponent ExpireSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
float ExpireSystem::getDuration(const ExpireComponent& cmpnt) const {
	return mDuration[cmpnt.index];
}

//	============================================================================
void ExpireSystem::setDuration(const ExpireComponent& cmpnt, float duration) {
	mDuration[cmpnt.index] = duration;
}

//	============================================================================
void ExpireSystem::update(World& world, float elapsedSeconds) {
	std::vector <Entity> removeEntities;

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		mDuration[index] -= elapsedSeconds;

		if (mDuration[index] <= 0.0f) {
			removeEntities.push_back(p.second);
		}
	}

	for (auto& entity : removeEntities) {
		world.destroyEntity(entity);
	}
}
}