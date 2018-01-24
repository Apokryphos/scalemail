#include "load_map.hpp"
#include "world.hpp"

namespace ScaleMail
{
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
    mMap = ScaleMail::loadMap("assets/maps/" + mapName + ".tmx");
}
}