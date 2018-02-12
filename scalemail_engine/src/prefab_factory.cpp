#include "bullet_util.hpp"
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
	gunSystem.setBulletImpactTilesetId(gunCmpnt, getBulletImpactTilesetId(3));
	gunSystem.setBulletTilesetId(gunCmpnt, getBulletTilesetId(3));
	gunSystem.setBulletLightColor(gunCmpnt, getBulletLightColor(3));
	gunSystem.setCooldownDuration(gunCmpnt, 0.1f);
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