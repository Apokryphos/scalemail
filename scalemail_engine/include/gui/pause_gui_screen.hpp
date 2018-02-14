#pragma once

#include "gui_screen.hpp"

namespace ScaleMail
{
class PauseGuiScreen : public GuiScreen
{
public:
	virtual void draw(Game& game, SpriteBatch& spriteBatch) override;
	virtual void initialize(AssetManager& assetManager) override;
};
}