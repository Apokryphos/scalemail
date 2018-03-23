#pragma once

#include "entity.hpp"
#include "game_state.hpp"
#include <vector>

namespace ScaleMail
{
class World;

class IntroGameState : public GameState
{
	bool mSkipIntro;
	bool mDoorsClosed;
	int mIntroState;
	float mIntroTicks;
	float mTextAlpha;

	std::vector<Entity> mDoorEntities;
	std::vector<Entity> mBuriedEntities;

	void updateState(World& world, float elapsedSeconds);

public:
	IntroGameState(GameStateManager& gameStateManager);
	virtual void activate(Game& game) override;
	void activate(Game& game, bool skipIntro);
	virtual void draw(const Game& game) override;
	virtual void initialize(Game& game) override;
	virtual void update(Game& game, float elapsedSeconds) override;
};
}