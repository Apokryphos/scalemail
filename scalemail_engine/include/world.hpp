#pragma once

#include "direction.hpp"
#include "door_system.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "light_system.hpp"
#include "map.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ScaleMail
{
class World
{
    EntityManager mEntityManager;
    std::shared_ptr<Map> mMap;

    PhysicsSystem mPhysicsSystem;
    SpriteSystem mSpriteSystem;
    LightSystem mLightSystem;
    NameSystem mNameSystem;
    DoorSystem mDoorSystem;

public:
    World();
    Entity createActor(float x, float y, int actorIndex,
                       Direction facing = Direction::SOUTH);
    Entity createBullet(glm::vec2 position, glm::vec2 direction, float speed,
                        int tilesetId);
    Entity createDoor(float x, float y, int openTilesetId, int closedTilesetId,
                      bool open, const std::string name = "");
    Entity createDoor(float x, float y, int openTilesetId,
                      int closedTilesetId, int childOpenTilesetId,
                      int childClosedTilesetId, bool open,
                      const std::string name);
    Entity createEntity();
    Entity createProp(glm::vec2 position, int tilesetId);
    DoorSystem& getDoorSystem();
    std::vector<Entity> getEntitiesByName(const std::string name) const;
    LightSystem& getLightSystem();
    Map* getMap();
    NameSystem& getNameSystem();
    PhysicsSystem& getPhysicsSystem();
    SpriteSystem& getSpriteSystem();
    void initialize(AssetManager* assetManager);
    void loadMap(const std::string& mapName);
    void update(float elapsedSeconds);
};
}