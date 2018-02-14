#pragma once

namespace ScaleMail
{
class AssetManager;
class Game;
class SpriteBatch;

class GuiScreen
{
	bool mVisible;

public:
	GuiScreen();
	virtual void draw(Game& game, SpriteBatch& spriteBatch) = 0;
	bool getVisible() const;
	virtual void initialize(AssetManager& assetManager) = 0;
	void setVisible(bool visible);
};
}