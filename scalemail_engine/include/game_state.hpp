#pragma once

namespace ScaleMail
{
class Camera;
struct Game;
class GameStateManager;

class GameState
{
	GameStateManager& mGameStateManager;

public:
	GameState(GameStateManager& gameStateManager);
	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
	virtual void activate(Game& game) = 0;
	virtual void draw(const Game& game) = 0;
	GameStateManager& getGameStateManager();
	virtual void initialize(Game& game) = 0;
	virtual void update(Game& game, float elapsedSeconds) = 0;
};
}