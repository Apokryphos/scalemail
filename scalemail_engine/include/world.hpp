#pragma once

#include "direction.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "map.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include <memory>

namespace ScaleMail
{
class World
{
    EntityManager mEntityManager;
    std::shared_ptr<Map> mMap;

    PhysicsSystem mPhysicsSystem;
    SpriteSystem mSpriteSystem;

public:
    World();
    Entity createActor(float x, float y, int actorIndex,
                       Direction facing = Direction::SOUTH);
    Entity createEntity();
    Entity createProp(glm::vec2 position, int tilesetId);
    Map* getMap();
    SpriteSystem& getSpriteSystem();
    void initialize(AssetManager* assetManager);
    void loadMap(const std::string& mapName);
    void update(float elapsedSeconds);
};
}