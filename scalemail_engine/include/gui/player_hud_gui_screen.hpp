#pragma once

#include "gui_screen.hpp"
#include "tileset.hpp"

namespace ScaleMail
{
class PlayerHudGuiScreen : public GuiScreen
{
	Tileset mGuiTileset;
	Tileset mItemTileset;

protected:
	virtual void draw(Game& game, SpriteBatch& spriteBatch) override;

public:
	virtual void initialize(AssetManager& assetManager) override;
};
}