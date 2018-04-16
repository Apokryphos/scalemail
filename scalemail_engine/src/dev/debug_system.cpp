#include "dev/debug_system.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
//	============================================================================
static DebugComponent makeComponent(const int index) {
	return DebugComponent(index);
}

//	============================================================================
DebugSystem::DebugSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void DebugSystem::createComponent() {
	const size_t cmpntIndex = mData.size();
	const Entity& entity = this->getEntityByComponentIndex(cmpntIndex);

	DebugComponentData data = {};
	data.entity = entity;
	data.idString = std::to_string(entity.id);

	mData.push_back(data);
}

//	============================================================================
void DebugSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
DebugComponent DebugSystem::getComponent(const Entity& entity) {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
const std::vector<DebugComponentData>& DebugSystem::getComponentData() const {
	return mData;
}

//	============================================================================
const std::string& DebugSystem::getIdString(const DebugComponent& cmpnt) const {
	return mData[cmpnt.index].idString;
}
}