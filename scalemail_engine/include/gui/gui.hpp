#pragma once

#include "gui/player_hud_gui_screen.hpp"
#include "sprite_batch.hpp"

namespace ScaleMail
{
class AssetManager;
class Camera;
class World;

class Gui
{
	PlayerHudGuiScreen mPlayerHudGuiScreen;
	SpriteBatch mSpriteBatch;

public:
	void draw(World& world, Camera& camera);
	void initialize(AssetManager& assetManager);
	void showPlayerHud(bool visible);
};
}