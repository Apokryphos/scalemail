#include "entity_manager.hpp"
#include "name_system.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
//	============================================================================
static NameComponent makeComponent(const int index) {
	return NameComponent(index);
}

//	============================================================================
NameSystem::NameSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mName.reserve(maxComponents);
}

//	============================================================================
void NameSystem::createComponent(const Entity& entity) {
	mName.push_back("");
}

//	============================================================================
void NameSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mName, index);
}

//	============================================================================
NameComponent NameSystem::getComponent(const Entity& entity) {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
std::vector<Entity> NameSystem::getEntitiesByName(const std::string name) const {
	std::vector<Entity> entities;

	for (int c = 0; c < mName.size(); ++c) {
		if (mName[c] == name) {
			entities.push_back(this->getEntityByComponentIndex(c));
		}
	}

	return entities;
}

//	============================================================================
std::string NameSystem::getName(const NameComponent& cmpnt) const {
    return mName[cmpnt.index];
}

//	============================================================================
void NameSystem::setName(const NameComponent& cmpnt, const std::string name) {
	mName[cmpnt.index] = name;
}
}