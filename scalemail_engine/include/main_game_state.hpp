#pragma once

#include "game_state.hpp"

namespace ScaleMail
{
class MainGameState : public GameState
{
public:
    MainGameState(GameStateManager& gameStateManager);
    virtual void activate(Game& game);
    virtual void draw(const Game& gameWindow, Camera& camera) override;
    virtual void initialize(Game& game);
    virtual void update(World& world, Camera& camera,
                        float elapsedSeconds) override;
};
}