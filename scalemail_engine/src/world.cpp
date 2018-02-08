#include "direction.hpp"
#include "load_map.hpp"
#include "sprite.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
World::World() : mPhysicsSystem(mEntityManager), mSpriteSystem(mEntityManager),
				 mLightSystem(mEntityManager),
				 mNameSystem(mEntityManager), mDoorSystem(mEntityManager) {
	mPlayers.emplace_back("player1");
	mPlayers.emplace_back("player2");
	mPlayers.emplace_back("player3");
	mPlayers.emplace_back("player4");
}

//  ============================================================================
Entity World::createActor(float x, float y, int actorIndex, Direction facing,
						  std::string name) {
	Entity entity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setTileset(spriteCmpnt, "actors");
	mSpriteSystem.setActorIndex(spriteCmpnt, actorIndex);
	mSpriteSystem.setFacing(spriteCmpnt, facing);

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, glm::vec2(x + 8.0f, y - 8.0f));
	mPhysicsSystem.setCollisionOffset(physicsCmpnt, glm::vec2(0.0f, 4.0f));

	if (name != "") {
		mNameSystem.addComponent(entity);
		NameComponent nameCmpnt = mNameSystem.getComponent(entity);
		mNameSystem.setName(nameCmpnt, name);
	}

	return entity;
}

//  ============================================================================
Entity World::createBullet(glm::vec2 position, glm::vec2 direction, float speed,
						   int tilesetId) {
	Entity entity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setDirection(spriteCmpnt, direction);
	mSpriteSystem.setSize(spriteCmpnt, glm::vec2(12.0f, 12.0f));
	mSpriteSystem.setTileset(spriteCmpnt, "fx");
	mSpriteSystem.setTilesetId(spriteCmpnt, tilesetId);

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, position);
	mPhysicsSystem.setDirection(physicsCmpnt, direction);
	mPhysicsSystem.setSpeed(physicsCmpnt, speed);

	const glm::vec4 lightColor(0.60f, 0.85f, 0.10f, 1.0f);
	const float lightSize = 16;
	const float lightGlowSize = lightSize * 0.33f;
	const float lightPulse = 32;
	const float lightPulseSize = 6;

	mLightSystem.addComponent(entity);
	LightComponent lightCmpnt = mLightSystem.getComponent(entity);
	mLightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
	mLightSystem.setColor(lightCmpnt, lightColor);
	mLightSystem.setGlowSize(lightCmpnt, lightGlowSize);
	mLightSystem.setSize(lightCmpnt, lightSize);
	mLightSystem.setPulse(lightCmpnt, lightPulse);
	mLightSystem.setPulseSize(lightCmpnt, lightPulseSize);

	return entity;
}

//  ============================================================================
Entity World::createDoor(float x, float y, int openTilesetId,
						 int closedTilesetId, bool open, const std::string name) {
	Entity entity = mEntityManager.createEntity();

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, glm::vec2(x + 8.0f, y - 8.0f));
	mPhysicsSystem.setRadius(physicsCmpnt, 0);

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setTileset(spriteCmpnt, "world");
	mSpriteSystem.setTilesetId(spriteCmpnt, open ? openTilesetId : closedTilesetId);

	mDoorSystem.addComponent(entity);
	DoorComponent doorCmpnt = mDoorSystem.getComponent(entity);
	mDoorSystem.setTilesetId(doorCmpnt, openTilesetId, closedTilesetId);
	mDoorSystem.setOpen(doorCmpnt, open);

	if (name != "") {
		mNameSystem.addComponent(entity);
		NameComponent nameCmpnt = mNameSystem.getComponent(entity);
		mNameSystem.setName(nameCmpnt, name);
	}

	return entity;
}

//  ============================================================================
Entity World::createDoor(float x, float y, int openTilesetId,
						 int closedTilesetId, int childOpenTilesetId,
						 int childClosedTilesetId, bool open,
						 const std::string name) {
	Entity entity = this->createDoor(x, y, openTilesetId, closedTilesetId,
									 open, name);

	DoorComponent doorCmpnt = mDoorSystem.getComponent(entity);
	mDoorSystem.setChildTilesetId(doorCmpnt, childOpenTilesetId,
								  childClosedTilesetId);

	return entity;
}

//  ============================================================================
Entity World::createEntity() {
	return mEntityManager.createEntity();
}

//  ============================================================================
Entity World::createProp(glm::vec2 position, int tilesetId, float offsetZ) {
	return this->createProp(position, tilesetId, tilesetId, offsetZ);
}

//  ============================================================================
Entity World::createProp(glm::vec2 position, int frame1TilesetId,
						 int frame2TilesetId, float offsetZ) {
	Entity entity = mEntityManager.createEntity();

	mSpriteSystem.addComponent(entity);
	SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
	mSpriteSystem.setOffsetZ(spriteCmpnt, offsetZ);
	mSpriteSystem.setTileset(spriteCmpnt, "world");
	mSpriteSystem.setTilesetId(spriteCmpnt, frame1TilesetId, frame2TilesetId);

	mPhysicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
	mPhysicsSystem.setPosition(physicsCmpnt, position + glm::vec2(8.0f, -8.0f));
	mPhysicsSystem.setRadius(physicsCmpnt, 0);

	return entity;
}

//  ============================================================================
DoorSystem& World::getDoorSystem() {
	return mDoorSystem;
}

//	============================================================================
std::vector<Entity> World::getEntitiesByName(const std::string name) const {
	return mNameSystem.getEntitiesByName(name);
}

//  ============================================================================
LightSystem& World::getLightSystem() {
	return mLightSystem;
}

//  ============================================================================
std::vector<Player*> World::getPlayers() {
	std::vector<Player*> players;

	for (auto& player : mPlayers) {
		players.push_back(&player);
	}

	return players;
}

//  ============================================================================
Map* World::getMap() {
	return mMap.get();
}

//  ============================================================================
NameSystem& World::getNameSystem() {
	return mNameSystem;
}

//  ============================================================================
PhysicsSystem& World::getPhysicsSystem() {
	return mPhysicsSystem;
}

//  ============================================================================
SpriteSystem& World::getSpriteSystem() {
	return mSpriteSystem;
}

//  ============================================================================
void World::initialize(AssetManager* assetManager) {
	mSpriteSystem.initialize(assetManager);
	mLightSystem.initialize(*assetManager);
	mDoorSystem.initialize(mPhysicsSystem, mSpriteSystem);
	mPhysicsSystem.initialize(*assetManager);
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
	mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);

	//  Assign players to map entities
	for (auto& player : mPlayers) {
		auto entities = mNameSystem.getEntitiesByName(player.name);

		if (!entities.empty()) {
			player.entity = entities.front();
		}
	}
}

//  ============================================================================
void World::update(float elapsedSeconds) {
	mPhysicsSystem.simulate(elapsedSeconds);
	mSpriteSystem.update(elapsedSeconds, mPhysicsSystem);
	mLightSystem.update(elapsedSeconds, mPhysicsSystem);
}
}