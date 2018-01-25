#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ScaleMail
{
class AssetManager;
struct GameWindow;

void drawCenterText(const glm::vec2 position, const std::string& text,
                    const glm::vec4 color, const float size);
void drawText(const glm::vec2 position, const std::string& text,
              const glm::vec3 color, const float size);
void initializeFont(AssetManager& assetManager);
void renderText(const GameWindow& gameWindow);
}