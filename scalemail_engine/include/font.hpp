#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace ScaleMail
{
class AssetManager;
struct GameWindow;
struct RenderOptions;

static const float NORMAL_FONT_SIZE = 8.0f;

void drawCenterText(const glm::vec2 position, const std::string& text,
					const glm::vec4 color, const float size);
void drawText(const glm::vec2 position, const std::string& text,
			  const glm::vec4 color, const float size);
void initializeFont(AssetManager& assetManager,
					const RenderOptions& renderOptions);
void renderText(const GameWindow& gameWindow);
}