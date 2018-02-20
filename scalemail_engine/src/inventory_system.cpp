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
	mData.emplace_back();
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
void InventorySystem::addItem(const InventoryComponent& cmpnt,
							  struct Item& item) {
	mData[cmpnt.index].items.push_back(item);
}

//	============================================================================
int InventorySystem::getItemCount(const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].items.size();
}

//	============================================================================
std::vector<Item> InventorySystem::getItems(
	const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].items;
}
}