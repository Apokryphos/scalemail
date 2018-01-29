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
}

//  ============================================================================
Entity World::createActor(float x, float y, int actorIndex, Direction facing) {
    Entity entity = mEntityManager.createEntity();

    mSpriteSystem.addComponent(entity);
    SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
    mSpriteSystem.setTileset(spriteCmpnt, "actors");
    mSpriteSystem.setActorIndex(spriteCmpnt, actorIndex);
    mSpriteSystem.setFacing(spriteCmpnt, facing);

    mPhysicsSystem.addComponent(entity);
    PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
    mPhysicsSystem.setPosition(physicsCmpnt, glm::vec2(x + 8.0f, y - 8.0f));
    return entity;
}

//  ============================================================================
Entity World::createBullet(glm::vec2 position, glm::vec2 direction, float speed,
                           int tilesetId) {
    Entity entity = mEntityManager.createEntity();

    mSpriteSystem.addComponent(entity);
    SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
    mSpriteSystem.setDirection(spriteCmpnt, direction);
    mSpriteSystem.setTileset(spriteCmpnt, "fx");
    mSpriteSystem.setTilesetId(spriteCmpnt, tilesetId);

    mPhysicsSystem.addComponent(entity);
    PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
    mPhysicsSystem.setPosition(physicsCmpnt, position + glm::vec2(8.0f, -8.0f));
    mPhysicsSystem.setDirection(physicsCmpnt, direction);
    mPhysicsSystem.setSpeed(physicsCmpnt, speed);

    const glm::vec4 lightColor(0.64f, 0.81f, 0.15f, 0.85f);
    const float lightSize = 64;
    const float lightGlowSize = lightSize * 0.25f;
    const float lightPulse = 8;
    const float lightPulseSize = 8;

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
Entity World::createProp(glm::vec2 position, int tilesetId) {
    Entity entity = mEntityManager.createEntity();

    mSpriteSystem.addComponent(entity);
    SpriteComponent spriteCmpnt = mSpriteSystem.getComponent(entity);
    mSpriteSystem.setTileset(spriteCmpnt, "world");
    mSpriteSystem.setTilesetId(spriteCmpnt, tilesetId);

    mPhysicsSystem.addComponent(entity);
    PhysicsComponent physicsCmpnt = mPhysicsSystem.getComponent(entity);
    mPhysicsSystem.setPosition(physicsCmpnt, position + glm::vec2(8.0f, -8.0f));
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
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
    mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);
}

//  ============================================================================
void World::update(float elapsedSeconds) {
    mPhysicsSystem.simulate(elapsedSeconds);
    mSpriteSystem.update(elapsedSeconds, mPhysicsSystem);
    mLightSystem.update(elapsedSeconds, mPhysicsSystem);
}
}