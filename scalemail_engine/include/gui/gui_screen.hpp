#pragma once

namespace ScaleMail
{
class AssetManager;
class Game;
class SpriteBatch;

class GuiScreen
{
	bool mVisible;
	float mAlpha;
	float mAlphaTicks;

protected:
	virtual void draw(Game& game, SpriteBatch& spriteBatch) = 0;

public:
	GuiScreen();
	void drawScreen(Game& game, SpriteBatch& spriteBatch);
	bool getVisible() const;
	virtual void initialize(AssetManager& assetManager) = 0;
	void setVisible(bool visible);
	void update(float elapsedSeconds);
};
}