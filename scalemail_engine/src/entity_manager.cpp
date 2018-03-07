#include "entity_manager.hpp"

namespace ScaleMail
{
//  ============================================================================
EntityManager::EntityManager() {
	mGenerations.reserve(MINIMUM_FREE_INDICES);
}

//  ============================================================================
Entity EntityManager::createEntity() {
	unsigned index;

	if (mFreeIndices.size() > MINIMUM_FREE_INDICES) {
		index = mFreeIndices.front();
		mFreeIndices.pop_front();
	} else {
		mGenerations.push_back(0);
		index = (int)mGenerations.size() - 1;
	}

	return makeEntity(index, mGenerations[index]);
}

//  ============================================================================
void EntityManager::destroyEntity(const Entity& entity) {
	const unsigned index = entity.index();
	++mGenerations[index];
	mFreeIndices.push_back(index);
}

//  ============================================================================
bool EntityManager::isAlive(const Entity& entity) const {
	return mGenerations[entity.index()] == entity.generation();
}
}