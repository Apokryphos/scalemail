#include "gui/player_hud_gui_screen.hpp"
#include "ease.hpp"
#include "math_util.hpp"
#include "sprite_batch.hpp"

namespace ScaleMail
{
static const float ALPHA_CHANGE_DURATION = 2.0f;

//	============================================================================
GuiScreen::GuiScreen() : mVisible(false), mAlpha(0.0f), mAlphaTicks(0.0f) {
}

//	============================================================================
void GuiScreen::drawScreen(Game& game, SpriteBatch& spriteBatch) {
	spriteBatch.setAlpha(mAlpha);
	this->draw(game, spriteBatch);
}

//	============================================================================
bool GuiScreen::getVisible() const {
	return mVisible;
}

//	============================================================================
void GuiScreen::setVisible(bool visible) {
	mVisible = visible;
}

//	============================================================================
void GuiScreen::update(float elapsedSeconds) {
	const float alphaDirection = mVisible ? 1.0f : -1.0f;
	const float alphaDelta = alphaDirection * elapsedSeconds;

	mAlphaTicks = clamp(
		mAlphaTicks + alphaDelta,
		0.0f,
		ALPHA_CHANGE_DURATION
	);

	const float alpha = easeOutCubic(
		mAlphaTicks, mAlpha, alphaDelta, ALPHA_CHANGE_DURATION);

	mAlpha = clamp(alpha, 0.0f, 1.0f);
}
}