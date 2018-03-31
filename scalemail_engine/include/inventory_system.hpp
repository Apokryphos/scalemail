#pragma once

#include "entity_system.hpp"
#include <memory>
#include <vector>

namespace ScaleMail
{
class Item;
class World;

struct InventoryComponent {
	InventoryComponent(const int index) { this->index = index; }
	int index;
};

struct InventoryComponentData
{
	int carryCapacity;
	std::vector<std::shared_ptr<Item>> itemSlots;
};

class InventorySystem : public EntitySystem
{
	World& mWorld;
	std::vector<InventoryComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	InventorySystem(World& world, EntityManager& entityManager,
					int maxComponents = 1000);
	InventoryComponent getComponent(const Entity& entity) const;
	bool addItem(const InventoryComponent& cmpnt, std::shared_ptr<Item> item);
	int getCarryCapacity(const InventoryComponent& cmpnt) const;
	int getItemCount(const InventoryComponent& cmpnt) const;
	const std::vector<std::shared_ptr<Item>>& getItems(
		const InventoryComponent& cmpnt) const;
	int isFull(const InventoryComponent& cmpnt) const;
	bool useItem(const InventoryComponent& cmpnt, std::shared_ptr<Item> item);
};
}