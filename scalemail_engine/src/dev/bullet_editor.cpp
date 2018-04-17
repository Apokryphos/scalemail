#include "bullet_data.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
void drawBulletDataEditor(BulletData& bulletData) {
	ImGui::InputInt("Tileset ID", &bulletData.tilesetId, 0, 512);
	ImGui::InputInt("Impact Tileset ID", &bulletData.impactTilesetId, 0, 512);
	ImGui::SliderFloat("Damage", &bulletData.damage, 0.0f, 1024.0f);
	ImGui::SliderFloat("Life", &bulletData.life, 0.0f, 100.0f);
	ImGui::SliderFloat("Offset", &bulletData.offset, 0.0f, 100.0f);
	ImGui::SliderFloat("Rotate Offset", &bulletData.rotateOffset, 0.0f, 100.0f);
	ImGui::SliderFloat("Speed", &bulletData.speed, 0.0f, 100.0f);
}
}
