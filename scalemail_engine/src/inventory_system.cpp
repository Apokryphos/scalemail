#include "health_system.hpp"
#include "entity_types.hpp"
#include "inventory_system.hpp"
#include "item.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
static InventoryComponent makeComponent(const int index) {
	return InventoryComponent(index);
}

//	============================================================================
InventorySystem::InventorySystem(World& world, EntityManager& entityManager,
								 int maxComponents)
	: EntitySystem(entityManager, maxComponents), mWorld(world) {
	mData.reserve(maxComponents);
}

//	============================================================================
bool InventorySystem::addItem(const InventoryComponent& cmpnt,
							  std::shared_ptr<Item> item) {
	if (item == nullptr) {
		return false;
	}

	auto& slots = mData[cmpnt.index].itemSlots;
	const size_t slotCount = slots.size();

	//	Find an empty item slot
	for (size_t n = 0; n < slotCount; ++n) {
		if (slots[n] == nullptr) {
			slots[n] = item;
			return true;
		}
	}

	return false;
}

//	============================================================================
void InventorySystem::createComponent() {
	InventoryComponentData data = {};
	data.carryCapacity = 3;
	data.itemSlots.resize(data.carryCapacity);

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
int InventorySystem::getCarryCapacity(const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].carryCapacity;
}

//	============================================================================
int InventorySystem::getItemCount(const InventoryComponent& cmpnt) const {
	const auto& slots = mData[cmpnt.index].itemSlots;
	const size_t slotCount = slots.size();
	size_t itemCount = 0;

	for (size_t n = 0; n < slotCount; ++n) {
		if (slots[n] != nullptr) {
			++itemCount;
		}
	}

	return itemCount;
}

//	============================================================================
const std::vector<std::shared_ptr<Item>>& InventorySystem::getItems(
	const InventoryComponent& cmpnt) const {
	return mData[cmpnt.index].itemSlots;
}

//	============================================================================
int InventorySystem::isFull(const InventoryComponent& cmpnt) const {
	return this->getItemCount(cmpnt) >= mData[cmpnt.index].carryCapacity;
}

//	============================================================================
bool InventorySystem::useItem(const InventoryComponent& cmpnt,
							  std::shared_ptr<Item> item) {
	if (item == nullptr) {
		return false;
	}

	auto& slots = mData[cmpnt.index].itemSlots;

	//	Ensure item exists in inventory
	auto find = std::find(slots.begin(), slots.end(), item);
	if (find == slots.end()) {
		return false;
	}

	Entity entity = this->getEntityByComponentIndex(cmpnt.index);

	//	Heal using item
	if (item->heal > 0.0f) {
		HealthSystem& healthSystem = mWorld.getHealthSystem();
		HealthComponent healthCmpnt = healthSystem.getComponent(entity);
		HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
		healthGauge.add(item->heal);

		PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		const glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

		//	Spawn healing effects
		createHealFx(mWorld, position);
	}

	//	Set item slot to null
	const size_t slotCount = slots.size();
	for (size_t n = 0; n < slotCount; ++n) {
		if (slots[n] == item) {
			slots[n] = nullptr;
		}
	}

	return true;
}
}