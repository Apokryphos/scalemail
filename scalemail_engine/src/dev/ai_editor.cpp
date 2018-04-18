#include "gui/imgui_ext.hpp"
#include "game.hpp"
#include "ai_system.hpp"
#include "world.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
void drawAiDataEditor(AiData& aiData) {
	ImGui::Checkbox("Avoid", &aiData.avoidEnabled);
	ImGui::Checkbox("Seek", &aiData.seekEnabled);
	ImGui::Checkbox("Wander", &aiData.wanderEnabled);
	ImGui::SliderFloat("Wander Radius", &aiData.arrivalRadius, 0.0f, 256.0f);
}

//  ============================================================================
void drawAiDebugDataEditor(AiDebugData& aiDebugData) {
	ImGui::Value("Wander Angle", aiDebugData.wanderAngle);
	ImGui::Value("Avoid Force", aiDebugData.avoidForce);
	ImGui::Value("Seek Force", aiDebugData.seekForce);
	ImGui::Value("Seek Target", aiDebugData.seekTarget);
	ImGui::Value("Wander Target", aiDebugData.wanderForce);
	ImGui::Value("Move Direction", aiDebugData.moveDirection);
}

//  ============================================================================
void drawAiComponentEditor(AiSystem& aiSystem, const Entity& entity) {
	if (aiSystem.hasComponent(entity)) {
		AiComponent aiCmpnt = aiSystem.getComponent(entity);

		AiData data = aiSystem.getAiData(aiCmpnt);
		drawAiDataEditor(data);
		aiSystem.setAiData(aiCmpnt, data);

		if (ImGui::Button("Remove Component")) {
			aiSystem.removeComponent(entity);
		}

		ImGui::Separator();

		AiDebugData debugData = aiSystem.getAiDebugData(aiCmpnt);
		drawAiDebugDataEditor(debugData);
	} else {
		if (ImGui::Button("Add Component")) {
			aiSystem.addComponent(entity);
		}
	}
}

//  ============================================================================
void drawAiComponentEditorWindow(AiSystem& aiSystem, const Entity& entity) {
	ImGui::Begin("Ai Editor");
	drawAiComponentEditor(aiSystem, entity);
	ImGui::End();
}
}
