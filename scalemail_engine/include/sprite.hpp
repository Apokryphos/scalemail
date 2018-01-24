#pragma once

#include "direction.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
class Camera;
struct GameWindow;

struct SpriteFrame {
    float duration;
    int tilesetIds[4];
};

struct SpriteAnimation {
    int frameIndex;
    float ticks;
    SpriteFrame frames[2];
};

struct Sprite {
    Direction facing;
    int textureId;
    float size;
    glm::vec2 position;
    glm::vec4 color;
    SpriteAnimation animation;
};

void addActorSprite(glm::vec2 position, int actorIndex,
                    Direction facing = Direction::SOUTH);
void addWorldSprite(glm::vec2 position, int tilesetId);
void initializeSprites();
void renderSprites(GameWindow& gameWindow, Camera& camera);
void simulateSprites(float elapsedSeconds);
}