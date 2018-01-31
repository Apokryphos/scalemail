#include "asset_manager.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "sprite_system.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace ScaleMail
{
static SpriteBatch spriteBatch;

//  ============================================================================
void getActorSpriteAnimation(const int actorIndex, SpriteAnimation& anim) {
	const int ACTORS_PER_ROW = 4;
	const int SPRITES_PER_ROW = 16;

	int spriteX = actorIndex % ACTORS_PER_ROW;
	int spriteY = actorIndex / ACTORS_PER_ROW * 2;

	int spriteIndex = spriteX * ACTORS_PER_ROW + spriteY * SPRITES_PER_ROW;

	const int east = static_cast<int>(Direction::EAST);
	const int south = static_cast<int>(Direction::SOUTH);
	const int north = static_cast<int>(Direction::NORTH);
	const int west = static_cast<int>(Direction::WEST);

	anim.frames[0].tilesetIds[east]  = spriteIndex;
	anim.frames[0].tilesetIds[south] = spriteIndex + 1;
	anim.frames[0].tilesetIds[north] = spriteIndex + 2;
	anim.frames[0].tilesetIds[west]  = spriteIndex + 3;

	anim.frames[1].tilesetIds[east]  = spriteIndex + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[south] = spriteIndex + 1 + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[north] = spriteIndex + 2 + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[west]  = spriteIndex + 3 + SPRITES_PER_ROW;
}

//  ============================================================================
void setSpriteAnimation(SpriteAnimation& animation, int frame1TilesetId,
						int frame2TilesetId) {
	const int east = static_cast<int>(Direction::EAST);
	const int north = static_cast<int>(Direction::NORTH);
	const int west = static_cast<int>(Direction::WEST);
	const int south = static_cast<int>(Direction::SOUTH);

	animation.frameIndex = 0;
	animation.ticks = 0;
	animation.frames[0].duration  = 0.3f;
	animation.frames[1].duration  = 0.3f;

	animation.frames[0].tilesetIds[east]  = frame1TilesetId;
	animation.frames[0].tilesetIds[north] = frame1TilesetId;
	animation.frames[0].tilesetIds[west]  = frame1TilesetId;
	animation.frames[0].tilesetIds[south] = frame1TilesetId;

	animation.frames[1].tilesetIds[east]  = frame1TilesetId;
	animation.frames[1].tilesetIds[north] = frame2TilesetId;
	animation.frames[1].tilesetIds[west]  = frame2TilesetId;
	animation.frames[1].tilesetIds[south] = frame2TilesetId;
}

//  ============================================================================
void setSpriteAnimationStatic(SpriteAnimation& animation, int tilesetId) {
	setSpriteAnimation(animation, tilesetId, tilesetId);
}

//  ============================================================================
void getWorldSpriteAnimation(const int tilesetId, SpriteAnimation& animation) {
	switch (tilesetId) {
		//  Torch
		case 197:
		//  Torch cast sprite
		case 199:
			setSpriteAnimation(animation, tilesetId, tilesetId + 1);
			break;

		//  Torch
		case 198:
		//  Torch cast sprite
		case 200:
			setSpriteAnimation(animation, tilesetId, tilesetId - 1);
			break;

		default:
			setSpriteAnimationStatic(animation, tilesetId);
			std::cerr << "getWorldSpriteAnimation case not implemented: "
					  << tilesetId << std::endl;
			break;
	}
}

//  ============================================================================
void initializeSprites(AssetManager& assetManager) {
	assetManager.loadTexture("actors");
	assetManager.loadTexture("fx");
	assetManager.loadTexture("world");
	spriteBatch.initialize(assetManager);
}

//  ============================================================================
void renderSprites(GameWindow& gameWindow, SpriteSystem& spriteSystem, Camera& camera) {
	glm::mat4 screenProjection =
		glm::ortho(0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
				   0.0f, 1.0f);

	glm::mat4 screenMvp = screenProjection * camera.getView();

	spriteBatch.begin();
	spriteSystem.buildVertexData(spriteBatch);
	spriteBatch.render(screenMvp);
	spriteBatch.end();
}
}