#include "imgui.h"

struct GLFWwindow;

namespace ScaleMail
{
class AssetManager;

IMGUI_API bool        ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks, AssetManager& assetManager);
IMGUI_API void        ImGui_ImplGlfw_Shutdown();
IMGUI_API void        ImGui_ImplGlfw_NewFrame();
IMGUI_API void        ImGui_ImplGlfw_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplGlfw_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplGlfw_CreateDeviceObjects();

// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
IMGUI_API void        ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
IMGUI_API void        ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);

void imGuiKeyCallback(GLFWwindow*, int key, int, int action, int mods);
void imGuiMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
}
