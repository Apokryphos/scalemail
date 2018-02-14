#pragma once

namespace ScaleMail
{
class AssetManager;
class Camera;
class SpriteBatch;
class World;

class GuiScreen
{
	bool mVisible;

public:
	GuiScreen();
	virtual void draw(World& world, Camera& camera,
					  SpriteBatch& spriteBatch) = 0;
	bool getVisible() const;
	virtual void initialize(AssetManager& assetManager) = 0;
	void setVisible(bool visible);
};
}