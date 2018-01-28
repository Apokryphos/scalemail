#pragma once

namespace ScaleMail
{
class Camera;
struct GameWindow;
class World;

class GameState
{
public:
    virtual void draw(const GameWindow& gameWindow, Camera& camera) = 0;
    virtual void initialize(World& world, Camera& camera) = 0;
    virtual void update(World& world, Camera& camera, float elapsedSeconds) = 0;
};
}