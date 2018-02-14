#include "asset_manager.hpp"
#include "gl_headers.hpp"
#include "gui/gui.hpp"

namespace ScaleMail
{
//	============================================================================
void Gui::draw(World& world, Camera& camera) {
	glDisable(GL_DEPTH_TEST);

	if (mPlayerHudGuiScreen.getVisible()) {
		mPlayerHudGuiScreen.draw(world, camera, mSpriteBatch);
	}
}

//	============================================================================
void Gui::initialize(AssetManager& assetManager) {
	mSpriteBatch.initialize(assetManager);
	mPlayerHudGuiScreen.initialize(assetManager);
}

//	============================================================================
void Gui::showPlayerHud(bool visible) {
	mPlayerHudGuiScreen.setVisible(visible);
}
}