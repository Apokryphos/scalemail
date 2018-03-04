#pragma once

#include "gui_screen.hpp"

namespace ScaleMail
{
class PauseGuiScreen : public GuiScreen
{
protected:
	virtual void draw(Game& game, SpriteBatch& spriteBatch) override;

public:
	virtual void initialize(AssetManager& assetManager) override;
};
}