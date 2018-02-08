#pragma once

namespace ScaleMail
{
class Camera;
struct Game;
class GameStateManager;
class World;

class GameState
{
	GameStateManager& mGameStateManager;

public:
	GameState(GameStateManager& gameStateManager);
	virtual void activate(Game& game) = 0;
	virtual void draw(const Game& game, Camera& camera) = 0;
	GameStateManager& getGameStateManager();
	virtual void initialize(Game& game) = 0;
	virtual void update(Game& game, float elapsedSeconds) = 0;
};
}