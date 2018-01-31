#pragma once

#include "entity_system.hpp"
#include <vector>

namespace ScaleMail
{
class PhysicsSystem;
class SpriteSystem;

struct DoorComponent {
	DoorComponent(const int index) { this->index = index; }
	int index;
};

struct DoorComponentData {
	bool open;
	bool childSprite;
	int openTilesetId;
	int closedTilesetId;
	int childOpenTilesetId;
	int childClosedTilesetId;
	Entity childEntity;
};

class DoorSystem : public EntitySystem
{
	PhysicsSystem* mPhysicsSystem;
	SpriteSystem* mSpriteSystem;

	std::vector<DoorComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	DoorSystem(EntityManager& entityManager, int maxComponents = 1000);
	DoorComponent getComponent(const Entity& entity);
	bool getOpen(const DoorComponent& cmpnt) const;
	void initialize(PhysicsSystem& physicsSystem, SpriteSystem& spriteSystem);
  	void setOpen(const DoorComponent& cmpnt, const bool open);
	void setChildTilesetId(const DoorComponent& cmpnt, const int openTilesetId,
						   const int closedTileset);
  	void setTilesetId(const DoorComponent& cmpnt, const int openTilesetId,
					  const int closedTileset);
	void simulate(float elapsedSeconds);
};
}