#pragma once

#include "gui_screen.hpp"
#include "tileset.hpp"

namespace ScaleMail
{
class PlayerHudGuiScreen : public GuiScreen
{
	Tileset mGuiTileset;

public:
	virtual void draw(Game& game, SpriteBatch& spriteBatch) override;
	virtual void initialize(AssetManager& assetManager) override;
};
}