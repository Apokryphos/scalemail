#pragma once

#include "direction.hpp"
#include "sprite_animation.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
class AssetManager;
class Camera;
struct GameWindow;
class SpriteSystem;

struct Sprite {
    Direction facing;
    int textureId;
    float size;
    glm::vec2 position;
    glm::vec4 color;
    SpriteAnimation animation;
};

void getActorSpriteAnimation(const int actorIndex, SpriteAnimation& anim);
void getWorldSpriteAnimation(const int index, SpriteAnimation& anim);
void initializeSprites(AssetManager& assetManager);
void renderSprites(GameWindow& gameWindow, SpriteSystem& spriteSystem,
                   Camera& camera);
void setSpriteAnimationStatic(SpriteAnimation& animation, int tilesetId);
}