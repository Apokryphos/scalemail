#pragma once

#include "game_state.hpp"

namespace ScaleMail
{
class World;

class IntroGameState : public GameState
{
	bool mDoorsClosed;
	int introState;
	float introTicks;
	float textAlpha;

	void updateState(World& world, Camera& camera, float elapsedSeconds);

public:
	IntroGameState(GameStateManager& gameStateManager);
	virtual void activate(Game& game) override;
	virtual void draw(const Game& game, Camera& camera) override;
	virtual void initialize(Game& game) override;
	virtual void update(Game& game, float elapsedSeconds) override;
};
}