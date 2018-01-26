#include "direction.hpp"
#include "load_map.hpp"
#include "sprite.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
World::World() : mPhysicsSystem(mEntityManager), mSpriteSystem(mEntityManager) {

}

//  ============================================================================
Entity World::createActor(float x, float y, int actorIndex, Direction facing ) {
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
Map* World::getMap() {
    return mMap.get();
}

//  ============================================================================
SpriteSystem& World::getSpriteSystem() {
    return mSpriteSystem;
}

//  ============================================================================
void World::initialize(AssetManager* assetManager) {
    mSpriteSystem.initialize(assetManager);
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
    mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);
}

//  ============================================================================
void World::update(float elapsedSeconds) {
    mSpriteSystem.update(elapsedSeconds, mPhysicsSystem);
}
}