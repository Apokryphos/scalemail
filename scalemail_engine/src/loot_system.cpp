#include "entity_collision.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "item.hpp"
#include "loot_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
static LootComponent makeComponent(const int index) {
	return LootComponent(index);
}

//	============================================================================
LootSystem::LootSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mLootable.reserve(maxComponents);
	mItem.reserve(maxComponents);
	mTargetEntity.reserve(maxComponents);
}

//	============================================================================
void LootSystem::createComponent() {
	mLootable.emplace_back(true);
	mItem.push_back({});
	mTargetEntity.emplace_back();
}

//	============================================================================
void LootSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mLootable, index);
	swapWithLastElementAndRemove(mItem, index);
	swapWithLastElementAndRemove(mTargetEntity, index);
}

//	============================================================================
LootComponent LootSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void LootSystem::initialize(InventorySystem& inventorySystem) {
	mInventorySystem = &inventorySystem;
}

//	============================================================================
void LootSystem::onEntityCollision(EntityCollision& collision) {
	if (collision.targetGroup != CollisionGroup::ITEM) {
		return;
	}

	collision.ignore = true;

	//	Only entities with inventory components can pick up loot
	if (!mInventorySystem->hasComponent(collision.sourceEntity)) {
		return;
	}

	InventoryComponent inventoryCmpnt =
		mInventorySystem->getComponent(collision.sourceEntity);

	//	Check if inventory is full
	if (mInventorySystem->isFull(inventoryCmpnt)) {
		return;
	}

	if (this->hasComponent(collision.targetEntity)) {
		LootComponent lootCmpnt = this->getComponent(collision.targetEntity);

		if (!mLootable[lootCmpnt.index]) {
			//	Item was already looted this frame
			return;
		}

		mLootable[lootCmpnt.index] = false;
		mTargetEntity[lootCmpnt.index] = collision.sourceEntity;
	}
}

//	============================================================================
void LootSystem::setItem(const LootComponent& cmpnt, std::shared_ptr<Item> item) {
	mItem[cmpnt.index] = item;
}

//	============================================================================
void LootSystem::simulate(World& world, [[maybe_unused]]float elapsedSeconds) {
	std::vector <Entity> removeEntities;

	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		if (!mLootable[index]) {
			InventoryComponent inventoryCmpnt =
				mInventorySystem->getComponent(mTargetEntity[index]);

			if (mInventorySystem->addItem(inventoryCmpnt, mItem[index])) {
				removeEntities.push_back(p.second);
			} else {
				//	Make item lootable again if adding it to inventory
				//	failed.
				mLootable[index] = true;
			}
		}
	}

	for (const auto& entity : removeEntities) {
		world.destroyEntity(entity);
	}
}
}