#pragma once

#include "entity_system.hpp"
#include "item.hpp"
#include <vector>

namespace ScaleMail
{
struct InventoryComponent {
	InventoryComponent(const int index) { this->index = index; }
	int index;
};

struct InventoryComponentData
{
	std::vector<Item> items;
};

class InventorySystem : public EntitySystem
{
	std::vector<InventoryComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	InventorySystem(EntityManager& entityManager, int maxComponents = 1000);
	InventoryComponent getComponent(const Entity& entity) const;
	void addItem(const InventoryComponent& cmpnt, struct Item& item);
	int getItemCount(const InventoryComponent& cmpnt) const;
	std::vector<Item> getItems(const InventoryComponent& cmpnt) const;
};
}