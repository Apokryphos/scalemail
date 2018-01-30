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
    void activateIntroGameState();
    void activateMainGameState();
    GameState* getActiveGameState();
    void initialize(Game& game);
};
}