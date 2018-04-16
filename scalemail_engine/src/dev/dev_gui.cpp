#include "dev/dev_gui.hpp"
#include "game.hpp"
#include "imgui.h"

namespace ScaleMail
{
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