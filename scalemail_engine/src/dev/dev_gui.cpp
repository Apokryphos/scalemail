#include "dev/dev_gui.hpp"
#include "dev/debug_system.hpp"
#include "bury_system.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "health_system.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "player.hpp"
#include "player_camera.hpp"
#include "world.hpp"
#include <imgui.h>

namespace ScaleMail
{
struct EntityInfo
{
	Entity entity;
	std::string idString;
};

//  ============================================================================
static bool debugDataVectorGetter(void* data, int n, const char** out_text) {
	const auto& v = *static_cast<std::vector<DebugComponentData>*>(data);
	*out_text = v[n].idString.c_str();
	return true;
}

//  ============================================================================
static void drawEntity(Entity entity, Game& game) {
	World& world = *game.world;

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
}

//  ============================================================================
static void drawPlayerEntityDebug(Game& game) {
	World& world = *game.world;

	const std::vector<Player*> players = world.getPlayers();

	//	ImGui requires non-const void pointer so cast const away...
	auto& debugComponentData =
		const_cast<std::vector<DebugComponentData>&>(
			world.getDebugSystem().getComponentData());

	static int listbox_item_current = 1;

	ImGui::Begin("Entity Debug");

	ImGui::Text("Entities:");

	//	Listbox of entities with debug components
	ImGui::PushItemWidth(100);
	ImGui::ListBox(
		"",
		&listbox_item_current,
		debugDataVectorGetter,
		static_cast<void*>(&debugComponentData),
		debugComponentData.size());
	ImGui::PopItemWidth();

	//	Debug info for selected entity
	ImGui::SameLine();
	const auto& entityInfo = debugComponentData[listbox_item_current];
	drawEntity(entityInfo.entity, game);

	ImGui::End();
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

	drawPlayerEntityDebug(game);
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