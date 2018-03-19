#pragma once

#include "entity.hpp"
#include "game_state.hpp"
#include <vector>

namespace ScaleMail
{
class World;

class IntroGameState : public GameState
{
	bool mDoorsClosed;
	int mIntroState;
	float mIntroTicks;
	float mTextAlpha;

	std::vector<Entity> mDoorEntities;
	std::vector<Entity> mBuriedEntities;

	void updateState(World& world, Camera& camera, float elapsedSeconds);

public:
	IntroGameState(GameStateManager& gameStateManager);
	virtual void activate(Game& game) override;
	virtual void draw(const Game& game, Camera& camera) override;
	virtual void initialize(Game& game) override;
	virtual void update(Game& game, float elapsedSeconds) override;
};
}