#include "dev/bullet_editor.hpp"
#include "dev/light_editor.hpp"
#include "bullet_data.hpp"
#include "bullet_util.hpp"
#include "gun_system.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
static BulletData makeDefaultBulletData() {
	BulletData bulletData = {};
	bulletData.tilesetId = getBulletTilesetId(0);
	bulletData.impactTilesetId = getBulletImpactTilesetId(0);
	bulletData.damage = 1.0f;
	bulletData.life = 1.0f;
	bulletData.speed = 8.0f;

	return bulletData;
}

//  ============================================================================
void drawGunComponentEditor(GunSystem& gunSystem, const Entity& entity) {
	if (gunSystem.hasComponent(entity)) {
		GunComponent gunCmpnt = gunSystem.getComponent(entity);

		if (ImGui::CollapsingHeader("Bullet")) {
			BulletData bulletData = gunSystem.getBulletData(gunCmpnt);
			drawBulletDataEditor(bulletData);
			gunSystem.setBulletData(gunCmpnt, bulletData);
		}

		if (ImGui::CollapsingHeader("Bullet Light")) {
			LightData lightData = gunSystem.getLightData(gunCmpnt);
			drawLightDataEditor(lightData);
			gunSystem.setLightData(gunCmpnt, lightData);
		}

		if (ImGui::Button("Remove Component")) {
			gunSystem.removeComponent(entity);
		}
	} else {
		if (ImGui::Button("Add Component")) {
			gunSystem.addComponent(entity);
			GunComponent gunCmpnt = gunSystem.getComponent(entity);
			gunSystem.setBulletData(gunCmpnt, makeDefaultBulletData());
		}
	}
}

//  ============================================================================
void drawGunComponentEditorWindow(GunSystem& gunSystem, const Entity& entity) {
	ImGui::Begin("Gun Editor");
	drawGunComponentEditor(gunSystem, entity);
	ImGui::End();
}
}
