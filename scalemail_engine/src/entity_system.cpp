#include "entity_system.hpp"
#include "entity_manager.hpp"
#include <cassert>

namespace ScaleMail
{
//	============================================================================
EntitySystem::EntitySystem(EntityManager& entityManager, int maxComponents)
	: mEntityManager(entityManager), maxComponents(maxComponents),
	  mComponentCount(0) {
	mComponentIndicesByEntity.reserve(maxComponents);
	mEntitiesByComponentIndices.reserve(maxComponents);
}

//	============================================================================
void EntitySystem::addComponent(const Entity& entity) {
	assert(mComponentIndicesByEntity.size() < this->maxComponents);

	int cmpntIndex = static_cast<int>(mComponentIndicesByEntity.size());

	mComponentIndicesByEntity.emplace(entity, cmpntIndex);
	mEntitiesByComponentIndices.emplace(cmpntIndex, entity);

	++mComponentCount;

	this->createComponent();
}

//	============================================================================
void EntitySystem::garbageCollect() {
	if (mComponentIndicesByEntity.size() > 0) {
		unsigned aliveInRow = 0;

		while (mComponentIndicesByEntity.size() > 0 && aliveInRow < 4) {
			//	Random index
			unsigned e = rand() % mComponentIndicesByEntity.size();

			auto pair = std::next(std::begin(mComponentIndicesByEntity), e);

			if (mEntityManager.isAlive(pair->first)) {
				++aliveInRow;
				continue;
			}

			aliveInRow = 0;

			this->removeComponent(pair->first);
		}
	}
}

//	============================================================================
const Entity& EntitySystem::getEntityByComponentIndex(const int index) const {
	return mEntitiesByComponentIndices.at(index);
}

//	============================================================================
EntityManager& EntitySystem::getEntityManager() { return mEntityManager; }

//	============================================================================
bool EntitySystem::hasComponent(const Entity& entity) const {
	return mComponentIndicesByEntity.find(entity) !=
		   mComponentIndicesByEntity.end();
}

//	============================================================================
void EntitySystem::removeComponent(const Entity& entity) {
	int last       = static_cast<int>(mComponentIndicesByEntity.size() - 1);
	int cmpntIndex = mComponentIndicesByEntity.at(entity);

	--mComponentCount;

	//	In each component list, swap the specified entity's component
	//	with the last component in the list
	this->destroyComponent(cmpntIndex);

	//	Update map to point to swapped component index
	//	Use at to ensure entry actually exists
	const Entity lastEntity = mEntitiesByComponentIndices.at(last);
	mComponentIndicesByEntity[lastEntity] = cmpntIndex;
	//	Erase after updating swapped component index in case the
	//	component being removed is the only one in the system.
	//	unordered_map.erase() doesn't fail if the specified key doesn't
	//	exist, so there's no need to check the size of the map before
	//	attempting to erase.
	mComponentIndicesByEntity.erase(entity);

	//	Have to check if the component being removed is the last one
	//	or else the map emplace call would add an entry for the removed
	//	component.
	mEntitiesByComponentIndices.erase(cmpntIndex);
	if (entity != lastEntity) {
		//	Erase the entry for the last entity in map
		mEntitiesByComponentIndices.erase(last);
		//	Add new entry for the former last entity and swapped component index
		mEntitiesByComponentIndices.emplace(cmpntIndex, lastEntity);
	}
}
}