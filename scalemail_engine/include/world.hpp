#pragma once

#include "direction.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "map.hpp"
#include <memory>

namespace ScaleMail
{
class World
{
    EntityManager mEntityManager;
    std::shared_ptr<Map> mMap;

public:
    Entity createActor(float x, float y, int actorIndex,
                       Direction facing = Direction::SOUTH);
    Entity createEntity();
    Map* getMap();
    void loadMap(const std::string& mapName);
};
}