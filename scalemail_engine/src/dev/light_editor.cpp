#include "game.hpp"
#include "math_util.hpp"
#include "light_data.hpp"
#include "light_system.hpp"
#include "world.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
static LightData makeDefaultLight() {
	LightData lightData = {};
	lightData.size = glm::vec2(16.0f);
	lightData.color = glm::vec4(1.0f);

	return lightData;
}

//  ============================================================================
void drawLightEditor(Game& game, const Entity& entity) {
	World& world = *game.world;

	ImGui::Begin("Light Editor");

	LightSystem& lightSystem = world.getLightSystem();
	if (lightSystem.hasComponent(entity)) {
		LightComponent lightCmpnt = lightSystem.getComponent(entity);

		LightData data = lightSystem.getLightData(lightCmpnt);

		ImGui::SliderFloat2("Size", &data.size[0], 0, 512.0f);
		ImGui::SliderFloat("Pulse", &data.pulse, 0.0f, 100.0f);
		ImGui::SliderFloat("Pulse Size", &data.pulseSize, 0.0f, 100.0f);
		ImGui::SliderFloat2("Glow Size", &data.glowSize[0], 0.0f, 100.0f);
		ImGui::ColorEdit4("Color", &data.color[0]);

		lightSystem.setLightData(lightCmpnt, data);
	} else {
		if (ImGui::Button("Add Component")) {
			lightSystem.addComponent(entity);
			LightComponent lightCmpnt = lightSystem.getComponent(entity);
			lightSystem.setLightData(lightCmpnt, makeDefaultLight());
		}
	}

	ImGui::End();
}
}
