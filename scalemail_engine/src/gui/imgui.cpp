#include "game_window.hpp"
#include "gui/imgui_glfw.hpp"
#include "gl_headers.hpp"
#include "imgui.h"

namespace ScaleMail
{
//	============================================================================
void initializeImgui(GLFWwindow* window, AssetManager& assetManager) {
	ImGui::CreateContext();
	ImGui_ImplGlfw_Init(window, true, assetManager);
	ImGui::StyleColorsDark();
}

//	============================================================================
void imGuiBeginFrame() {
	ImGui_ImplGlfw_NewFrame();
}

//	============================================================================
void imGuiEndFrame(GameWindow& gameWindow) {
	glViewport(0, 0, gameWindow.getWidth(), gameWindow.getHeight());
	ImGui::Render();
	ImGui_ImplGlfw_RenderDrawData(ImGui::GetDrawData());
}

//	============================================================================
void shutdownImGui() {
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
}
