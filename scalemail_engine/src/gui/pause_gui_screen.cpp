#include "gui/pause_gui_screen.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "font.hpp"
#include "game.hpp"
#include "sprite_batch.hpp"
#include "world.hpp"
#include <glm/mat4x4.hpp>

namespace ScaleMail
{
//	============================================================================
void PauseGuiScreen::draw(Game& game, [[maybe_unused]]SpriteBatch& spriteBatch) {
	const GameWindow& gameWindow = game.gameWindow;
	const Camera& camera = *game.camera;

	const float textSize = 8.0f * camera.getZoom();
	const float centerX = gameWindow.width * 0.5f;
	const float centerY = gameWindow.height * 0.5f - textSize;

	if (game.paused) {
		drawCenterText(
			glm::vec2(centerX, centerY),
			"PAUSED",
			glm::vec4(1.0f),
			textSize);
	}
}

//	============================================================================
void PauseGuiScreen::initialize([[maybe_unused]] AssetManager& assetManager) {
}
}