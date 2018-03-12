#pragma once

#include "direction.hpp"
#include "sprite_animation.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
class AssetManager;
class Camera;
struct RenderOptions;
class SpriteSystem;
class SpriteEffectSystem;

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
void renderSprites(
	SpriteSystem& spriteSystem,
	SpriteEffectSystem& spriteEffectSystem,
	Camera& camera);
void setSpriteAnimation(SpriteAnimation& animation,
						std::vector<int> frameTilesetIds);
}