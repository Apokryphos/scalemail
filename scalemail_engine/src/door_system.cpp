#include "entity_manager.hpp"
#include "door_system.hpp"
#include "physics_system.hpp"
#include "sprite.hpp"
#include "sprite_system.hpp"
#include "vector_util.hpp"
#include <algorithm>
#include <cassert>

namespace ScaleMail
{
//	============================================================================
static DoorComponent makeComponent(const int index) {
	return DoorComponent(index);
}

//	============================================================================
DoorSystem::DoorSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void DoorSystem::createComponent() {
	DoorComponentData data;
	data.open = true;
	data.childSprite = false;
	data.openTilesetId = 160;
	data.closedTilesetId = 161;
	data.childOpenTilesetId = 163;
	data.childClosedTilesetId = 165;

	mData.push_back(data);
}

//	============================================================================
void DoorSystem::destroyComponent(int index) {
	if (mData[index].childSprite) {
		mEntityManager.destroyEntity(mData[index].childEntity);
	}

	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
DoorComponent DoorSystem::getComponent(const Entity& entity) {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
bool DoorSystem::getOpen(const DoorComponent& cmpnt) const {
	return mData[cmpnt.index].open;
}

//	============================================================================
void DoorSystem::initialize(PhysicsSystem& physicsSystem,
							SpriteSystem& spriteSystem) {
	mPhysicsSystem = &physicsSystem;
	mSpriteSystem = &spriteSystem;
}

//	============================================================================
void DoorSystem::setChildTilesetId(const DoorComponent& cmpnt,
								   const int openTilesetId,
								   const int closedTileset) {
	DoorComponentData& data = mData[cmpnt.index];

	if (!data.childSprite) {
		data.childSprite = true;
		data.childEntity = mEntityManager.createEntity();
		mPhysicsSystem->addComponent(data.childEntity);
		mSpriteSystem->addComponent(data.childEntity);
	}

	data.childOpenTilesetId = openTilesetId;
	data.childClosedTilesetId = closedTileset;

	int tilesetId = data.open ? openTilesetId : closedTileset;

	SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(data.childEntity);
	mSpriteSystem->setTileset(spriteCmpnt, "world");
	mSpriteSystem->setTilesetId(spriteCmpnt, { tilesetId });

	Entity entity = getEntityByComponentIndex(cmpnt.index);
	PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);
	glm::vec2 position = mPhysicsSystem->getPosition(physicsCmpnt);

	physicsCmpnt = mPhysicsSystem->getComponent(data.childEntity);
	mPhysicsSystem->setPosition(physicsCmpnt, position + glm::vec2(0, -16.0f));
}

//	============================================================================
void DoorSystem::setOpen(const DoorComponent& cmpnt, const bool open) {
	DoorComponentData& data = mData[cmpnt.index];

	if (data.open == open) {
		return;
	}

	data.open = open;

	Entity entity = this->getEntityByComponentIndex(cmpnt.index);

	int tilesetId = data.open ?
		data.openTilesetId : data.closedTilesetId;

	SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);
	mSpriteSystem->setTilesetId(spriteCmpnt, { tilesetId });

	PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);
	mPhysicsSystem->setRadius(physicsCmpnt, open ? 0.0f : 8.0f);

	if (data.childSprite) {
		int childTilesetId = data.open ?
			data.childOpenTilesetId : data.childClosedTilesetId;

		SpriteComponent childSpriteCmpnt =
			mSpriteSystem->getComponent(data.childEntity);
		mSpriteSystem->setTilesetId(childSpriteCmpnt, { childTilesetId });

		PhysicsComponent childPhysicsCmpnt =
			mPhysicsSystem->getComponent(data.childEntity);
		mPhysicsSystem->setRadius(childPhysicsCmpnt, 0.0f);

	}
}

//	============================================================================
void DoorSystem::setTilesetId(const DoorComponent& cmpnt,
							  const int openTilesetId, const int closedTileset) {
	mData[cmpnt.index].openTilesetId = openTilesetId;
	mData[cmpnt.index].closedTilesetId = closedTileset;
}
}