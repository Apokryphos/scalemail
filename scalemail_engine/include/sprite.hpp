#pragma once

#include "direction.hpp"
#include "sprite_animation.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
class AssetManager;
class Camera;
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
void initializeSprites(AssetManager& assetManager);
void renderSprites(SpriteSystem& spriteSystem, Camera& camera);
void setSpriteAnimation(SpriteAnimation& animation, int frame1TilesetId,
	int frame2TilesetId);
}