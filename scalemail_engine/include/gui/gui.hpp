#pragma once

#include "gui/pause_gui_screen.hpp"
#include "gui/player_hud_gui_screen.hpp"
#include "sprite_batch.hpp"

namespace ScaleMail
{
class AssetManager;
class Game;

class Gui
{
	PauseGuiScreen mPauseGuiScreen;
	PlayerHudGuiScreen mPlayerHudGuiScreen;
	SpriteBatch mSpriteBatch;

public:
	void draw(Game& game);
	void initialize(AssetManager& assetManager);
	void showPlayerHud(bool visible);
	void update(float elapsedSeconds);
};
}