#pragma once

#include "intro_game_state.hpp"
#include "main_game_state.hpp"

namespace ScaleMail
{
class AssetManager;
class Game;
class GameState;

class GameStateManager
{
	Game* mGame;
	GameState* mActiveGameState;

	IntroGameState mIntroGameState;
	MainGameState mMainGameState;

public:
	GameStateManager();
	GameStateManager(const GameStateManager&) = delete;
	GameStateManager& operator=(const GameStateManager&) = delete;
	void activateIntroGameState();
	void activateMainGameState();
	GameState* getActiveGameState();
	void initialize(Game& game);
};
}