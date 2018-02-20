#pragma once

#include "entity_system.hpp"
#include "item.hpp"
#include <vector>

namespace ScaleMail
{
struct EntityCollision;
struct InventorySystem;
class World;

struct LootComponent {
	LootComponent(const int index) { this->index = index; }
	int index;
};

class LootSystem : public EntitySystem
{
	std::vector<float> mLootable;
	std::vector<Entity> mTargetEntity;
	std::vector<Item> mItem;

	InventorySystem* mInventorySystem;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	LootSystem(EntityManager& entityManager, int maxComponents = 1000);
	LootComponent getComponent(const Entity& entity) const;
	void initialize(InventorySystem& inventorySystem);
	void onEntityCollision(EntityCollision& collision);
	void setItem(const LootComponent& cmpnt, struct Item& item);
	void simulate(World& world, float elapsedSeconds);
};
}