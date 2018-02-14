#include "asset_manager.hpp"
#include "game.hpp"
#include "gl_headers.hpp"
#include "gui/gui.hpp"

namespace ScaleMail
{
//	============================================================================
void Gui::draw(Game& game) {
	glDisable(GL_DEPTH_TEST);

	mPauseGuiScreen.draw(game, mSpriteBatch);

	if (mPlayerHudGuiScreen.getVisible()) {
		mPlayerHudGuiScreen.draw(game, mSpriteBatch);
	}
}

//	============================================================================
void Gui::initialize(AssetManager& assetManager) {
	mSpriteBatch.initialize(assetManager);
	mPauseGuiScreen.initialize(assetManager);
	mPlayerHudGuiScreen.initialize(assetManager);
}

//	============================================================================
void Gui::showPlayerHud(bool visible) {
	mPlayerHudGuiScreen.setVisible(visible);
}
}