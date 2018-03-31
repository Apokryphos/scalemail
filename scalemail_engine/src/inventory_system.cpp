#include "inventory_system.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
//	============================================================================
static InventoryComponent makeComponent(const int index) {
	return InventoryComponent(index);
}

//	============================================================================
InventorySystem::InventorySystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void InventorySystem::createComponent() {
	InventoryComponentData data = {};
	data.carryCapacity = 3;

	mData.emplace_back(data);
}

//	============================================================================
void InventorySystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
InventoryComponent InventorySystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
bool InventorySystem::addItem(const InventoryComponent& cmpnt,
							  struct Item& item) {
	InventoryComponentData& data = mData[cmpnt.index];

	const size_t itemCount = data.items.size();

	if (data.carryCapacity > 0 &&
		itemCount < static_cast<size_t>(data.carryCapacity)) {
		mData[cmpnt.index].items.push_back(item);
		return true;
	}

	return false;
}

//	============================================================================
int InventorySystem::getCarryCapacity(const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].carryCapacity;
}

//	============================================================================
int InventorySystem::getItemCount(const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].items.size();
}

//	============================================================================
const std::vector<Item>& InventorySystem::getItems(
	const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].items;
}

//	============================================================================
int InventorySystem::isFull(const InventoryComponent& cmpnt) const {
	const InventoryComponentData& data = mData[cmpnt.index];
	return
		data.carryCapacity <= 0 ||
		(data.items.size() >= static_cast<size_t>(data.carryCapacity));
}
}