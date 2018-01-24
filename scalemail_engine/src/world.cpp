#include "direction.hpp"
#include "load_map.hpp"
#include "sprite.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
Entity World::createActor(float x, float y, int actorIndex, Direction facing ) {
    Entity entity = mEntityManager.createEntity();
    addActorSprite(glm::vec2(x + 8.0f, y - 8.0f), actorIndex, facing);
    return entity;
}

//  ============================================================================
Entity World::createEntity() {
    return mEntityManager.createEntity();
}

//  ============================================================================
Map* World::getMap() {
    return mMap.get();
}

//  ============================================================================
void World::loadMap(const std::string& mapName) {
    mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx", *this);
}
}