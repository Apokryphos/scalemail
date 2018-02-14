#include "gui/player_hud_gui_screen.hpp"

namespace ScaleMail
{
//	============================================================================
GuiScreen::GuiScreen() : mVisible(false) {
}

//	============================================================================
bool GuiScreen::getVisible() const {
	return mVisible;
}

//	============================================================================
void GuiScreen::setVisible(bool visible) {
	mVisible = visible;
}
}