#include "gui/imgui_ext.hpp"
#include "physics_system.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
void drawPhysicsDataEditor(PhysicsData& data) {
	ImGui::InputFloat2("Position", &data.position[0]);
	ImGui::SliderFloat("Radius", &data.radius, 0.0f, 100.0f);
	ImGui::SliderFloat("MaxSpeed", &data.maxSpeed, 0.0f, 100.0f);
	ImGui::SliderFloat("Acceleration", &data.acceleration, 0.0f, 100.0f);

	ImGui::SliderFloat2(
		"Direction",
		&data.direction[0],
		-1.0f,
		1.0f);

	ImGui::Checkbox("Ignore Actor Obstacles", &data.ignoreActorCollisions);
}

//  ============================================================================
void drawPhysicsDebugDataEditor(PhysicsDebugData& data) {
	ImGui::Value("Speed", data.speed);
	ImGui::Value("Velocity", data.velocity);
	ImGui::Value("Force", data.force);
}

//  ============================================================================
void drawPhysicsComponentEditor(PhysicsSystem& physicsSystem,
								 const Entity& entity) {
	if (physicsSystem.hasComponent(entity)) {
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);

		PhysicsData data = physicsSystem.getPhysicsData(physicsCmpnt);
		drawPhysicsDataEditor(data);
		physicsSystem.setPhysicsData(physicsCmpnt, data);

		if (ImGui::Button("Remove Component")) {
			physicsSystem.removeComponent(entity);
		}

		ImGui::Separator();

		PhysicsDebugData debugData = physicsSystem.getPhysicsDebugData(physicsCmpnt);
		drawPhysicsDebugDataEditor(debugData);
	} else {
		if (ImGui::Button("Add Component")) {
			physicsSystem.addComponent(entity);
		}
	}
}

//  ============================================================================
void drawPhysicsComponentEditorWindow(PhysicsSystem& physicsSystem,
									   const Entity& entity) {
	ImGui::Begin("Physics Editor");
	drawPhysicsComponentEditor(physicsSystem, entity);
	ImGui::End();
}
}
