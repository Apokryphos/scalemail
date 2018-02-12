#include "gun_system.hpp"
#include "prefab_factory.hpp"
#include "string_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
static void buildVampire(Entity entity, World& world) {
	GunSystem& gunSystem = world.getGunSystem();
	GunComponent gunCmpnt = gunSystem.getComponent(entity);
	gunSystem.setBulletTilesetId(gunCmpnt, 48);
}

//  ============================================================================
void PrefabFactory::buildPrefab(Entity entity, std::string prefabName,
								World& world) {
	prefabName = toLowercase(prefabName);

	if (prefabName == "vampire") {
		buildVampire(entity, world);
	}
}
}