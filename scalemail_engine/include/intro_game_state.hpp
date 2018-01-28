#pragma once

#include "game_state.hpp"

namespace ScaleMail
{
class IntroGameState : public GameState
{
    int introState;
    float introTicks;
    float textAlpha;

    void updateState(World& world, Camera& camera, float elapsedSeconds);

public:
    IntroGameState();
    virtual void draw(const GameWindow& gameWindow, Camera& camera) override;
    virtual void initialize(World& world, Camera& camera);
    virtual void update(World& world, Camera& camera,
                        float elapsedSeconds) override;
};
}