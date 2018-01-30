#pragma once

#include "game_state.hpp"

namespace ScaleMail
{
class MainGameState : public GameState
{
public:
    MainGameState();
    virtual void draw(const Game& gameWindow, Camera& camera) override;
    virtual void initialize(World& world, Camera& camera);
    virtual void update(World& world, Camera& camera,
                        float elapsedSeconds) override;
};
}