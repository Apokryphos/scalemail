struct GLFWwindow;

namespace ScaleMail
{
class AssetManager;
class GameWindow;

void imGuiBeginFrame();
void imGuiEndFrame(GameWindow& gameWindow);
void initializeImgui(GLFWwindow* window, AssetManager& assetManager);
void shutdownImGui();
}
