#pragma once

#include "gui/pause_gui_screen.hpp"
#include "gui/player_hud_gui_screen.hpp"
#include "sprite_batch.hpp"

namespace ScaleMail
{
class AssetManager;
class Game;
struct RenderOptions;

class Gui
{
	PauseGuiScreen mPauseGuiScreen;
	PlayerHudGuiScreen mPlayerHudGuiScreen;
	SpriteBatch mSpriteBatch;

public:
	Gui() = default;
	Gui(const Gui&) = delete;
	Gui& operator=(const Gui&) = delete;
	void draw(Game& game);
	void initialize(AssetManager& assetManager, const RenderOptions& renderOptions);
	void showPlayerHud(bool visible);
	void update(float elapsedSeconds);
};
}