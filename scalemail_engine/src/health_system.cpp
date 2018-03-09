#include "entity_types.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
static void dropLoot(World& world, const Entity& entity) {
	InventorySystem& inventorySystem = world.getInventorySystem();

	if (!inventorySystem.hasComponent(entity)) {
		return;
	}

	InventoryComponent inventoryCmpnt = inventorySystem.getComponent(entity);

	if (inventorySystem.getItemCount(inventoryCmpnt) <= 0) {
		return;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	const glm::vec2 size(16.0f);

	std::vector<Item> items = inventorySystem.getItems(inventoryCmpnt);

	const unsigned int itemCount = items.size();
	const float radius = 16.0f;

	for (unsigned int n = 0; n < itemCount; ++n) {
		Item& item = items[n];

		glm::vec2 itemPosition(
			position.x + radius * std::cos(n * TWO_PI / itemCount),
			position.y + radius * std::sin(n * TWO_PI / itemCount)
		);

		createLoot(world, itemPosition, size, item.tilesetId, item.name,
				   item.prefab);
	}
}

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
const HealthGauge& HealthSystem::getHealthGauge(
	const HealthComponent& cmpnt) const  {
	return mData[cmpnt.index].healthGauge;
}

//	============================================================================
HealthGauge& HealthSystem::getHealthGauge(const HealthComponent& cmpnt) {
	return mData[cmpnt.index].healthGauge;
}

//	============================================================================
void HealthSystem::setRespawn(const HealthComponent& cmpnt, const bool respawn) {
	mData[cmpnt.index].respawn = respawn;
}

//	============================================================================
void HealthSystem::update(World& world) {
	std::vector<Entity> removeEntities;

	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;
		const Entity& entity = p.second;

		if (mData[index].healthGauge.isEmpty()) {
			//	Only destroy entities if they don't respawn
			if (!mData[index].respawn) {
				dropLoot(world, entity);
				removeEntities.push_back(p.second);
			} else {
				//	Fill health gauge for now...
				mData[index].healthGauge.fill();
			}
		}
	}

	for (const auto& entity : removeEntities) {
		world.destroyEntity(entity);
	}
}
}