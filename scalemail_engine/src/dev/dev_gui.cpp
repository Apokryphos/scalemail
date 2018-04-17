#include "dev/dev_gui.hpp"
#include "dev/debug_system.hpp"
#include "dev/entity_list_box.hpp"
#include "dev/gun_editor.hpp"
#include "dev/light_editor.hpp"
#include "dev/particle_editor.hpp"
#include "bury_system.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "health_system.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "team_system.hpp"
#include "team_util.hpp"
#include "player.hpp"
#include "player_camera.hpp"
#include "world.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
static void drawEntityDebugWindow(Game& game, const Entity& entity) {
	World& world = *game.world;

	ImGui::Begin("Entity Debug");
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("ID: %d", entity.id);

	NameSystem& nameSystem = world.getNameSystem();
	if (nameSystem.hasComponent(entity)) {
		NameComponent nameCmpnt = nameSystem.getComponent(entity);
		std::string name = nameSystem.getName(nameCmpnt);
		ImGui::Text("Name: %s", name.c_str());
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	if (physicsSystem.hasComponent(entity)) {
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);
		ImGui::Text("Position: %.2f, %.2f", position.x, position.y);
	}

	TeamSystem& teamSystem = world.getTeamSystem();
	if (teamSystem.hasComponent(entity)) {
		TeamComponent teamCmpnt = teamSystem.getComponent(entity);
		const Team team = teamSystem.getTeam(teamCmpnt);
		ImGui::Text("Team: %s", teamToString(team).c_str());
	}

	HealthSystem& healthSystem = world.getHealthSystem();
	if (healthSystem.hasComponent(entity)) {
		HealthComponent healthCmpnt = healthSystem.getComponent(entity);
		const HealthGauge& health = healthSystem.getHealthGauge(healthCmpnt);
		ImGui::Text("Health: %.2f / %.2f", health.getValue(), health.getMax());
	}

	BurySystem& burySystem = world.getBurySystem();
	if (burySystem.hasComponent(entity)) {
		BuryComponent buryCmpnt = burySystem.getComponent(entity);
		BuryState buryState = burySystem.getBuryState(buryCmpnt);
		ImGui::Text("Bury State: %s", buryStateToString(buryState).c_str());
	}

	if (ImGui::Button("Focus")) {
		setPlayerCameraFollowEntity(entity, game);
	}

	ImGui::EndGroup();
	ImGui::End();
}

//  ============================================================================
static void drawEntityEditorWindows(Game& game, const Entity& entity) {
	World& world = *game.world;

	drawParticleComponentEditorWindow(world.getParticleSystem(), entity);
	drawLightComponentEditorWindow(world.getLightSystem(), entity);
	drawGunComponentEditorWindow(world.getGunSystem(), entity);
}

//  ============================================================================
static void updateEntitySelections(DebugSystem& debugSystem,
								   const Entity& selectedEntity) {
	if (!debugSystem.hasComponent(selectedEntity)) {
		return;
	}

	const DebugComponent debugCmpnt = debugSystem.getComponent(selectedEntity);
	debugSystem.setSelected(debugCmpnt, true);
}

//  ============================================================================
DevGui::DevGui() : mVisible(true) {
}

//  ============================================================================
void DevGui::draw(Game& game) {
	DevOptions& devOptions = game.devOptions;

	ImGui::Begin("Debug Drawing");
	ImGui::Checkbox("3D Camera", &devOptions.camera3d);
	ImGui::Checkbox("AI Forces", &devOptions.drawAiForces);
	ImGui::Checkbox("AI Obstacles", &devOptions.drawAiObstacles);
	ImGui::Checkbox("Collision Shapes", &devOptions.drawCollision);
	ImGui::Checkbox("Hide GUI", &devOptions.hideGui);
	ImGui::Checkbox("Triggers", &devOptions.drawTriggers);
	ImGui::End();

	ImGui::Begin("FPS");
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();

	this->drawEntityDebugWindows(game);
}

//  ============================================================================
void DevGui::drawEntityDebugWindows(Game& game) {
	World& world = *game.world;

	DebugSystem& debugSystem = world.getDebugSystem();

	debugSystem.clearSelected();

	mEntityListBox.setEntities(debugSystem.getComponentData());

	ImGui::Begin("Entity Debug");
	ImGui::Text("Entities:");
	mEntityListBox.draw();
	ImGui::End();

	if (mEntityListBox.hasSelectedEntity()) {
		Entity selectedEntity = mEntityListBox.getSelectedEntity();
		drawEntityDebugWindow(game, selectedEntity);
		drawEntityEditorWindows(game, selectedEntity);
		updateEntitySelections(debugSystem, selectedEntity);
	}
}

//  ============================================================================
bool DevGui::getKeyboardInputConsumed() const {
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}

//  ============================================================================
bool DevGui::getMouseInputConsumed() const {
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

//  ============================================================================
bool DevGui::getVisible() const {
	return mVisible;
}

//  ============================================================================
void DevGui::setVisible(const bool visible) {
	mVisible = visible;
}
}