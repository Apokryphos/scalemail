#include "asset_manager.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "sprite_effect_system.hpp"
#include "sprite_system.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
static SpriteBatch spriteBatch;
static Tileset guiTileset;

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

	anim.frameCount = 2;
	anim.frames.resize(2);

	anim.frames[0].duration  = 0.3f;
	anim.frames[0].tilesetIds[east]  = spriteIndex;
	anim.frames[0].tilesetIds[south] = spriteIndex + 1;
	anim.frames[0].tilesetIds[north] = spriteIndex + 2;
	anim.frames[0].tilesetIds[west]  = spriteIndex + 3;

	anim.frames[1].duration  = 0.3f;
	anim.frames[1].tilesetIds[east]  = spriteIndex + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[south] = spriteIndex + 1 + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[north] = spriteIndex + 2 + SPRITES_PER_ROW;
	anim.frames[1].tilesetIds[west]  = spriteIndex + 3 + SPRITES_PER_ROW;
}

//  ============================================================================
void setSpriteAnimation(SpriteAnimation& animation,
						std::vector<int> frameTilesetIds) {
	const int east = static_cast<int>(Direction::EAST);
	const int north = static_cast<int>(Direction::NORTH);
	const int west = static_cast<int>(Direction::WEST);
	const int south = static_cast<int>(Direction::SOUTH);

	animation.frameCount = frameTilesetIds.size();
	animation.frameIndex = 0;
	animation.ticks = 0;

	animation.frames.resize(frameTilesetIds.size());

	int f = 0;
	for (auto frameTilesetId : frameTilesetIds) {
		animation.frames[f].duration  = 0.3f;

		animation.frames[f].tilesetIds[east]  = frameTilesetId;
		animation.frames[f].tilesetIds[north] = frameTilesetId;
		animation.frames[f].tilesetIds[west]  = frameTilesetId;
		animation.frames[f].tilesetIds[south] = frameTilesetId;

		++f;
	}
}

//  ============================================================================
void initializeSprites(AssetManager& assetManager) {
	assetManager.loadTexture("world");
	assetManager.loadTexture("actors");
	assetManager.loadTexture("fx");

	guiTileset = assetManager.getTileset("gui");

	spriteBatch.initialize(assetManager);
}

//  ============================================================================
static void drawHealthGauge(glm::vec2 position, const float scale,
							float healthValue, float healthMax) {
	const float healthPercent = healthValue / healthMax;

	//	Positions for second and third tile for bottom (empty) layer
	glm::vec2 position2 = position + glm::vec2(16.0f, 0.0f);
	glm::vec2 position3 = position2 + glm::vec2(16.0f, 0.0f);

	//	Gauge image is three tiles in width (16x48 pixels)
	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uvDiscard;

	//	Get the top left of the first tile
	guiTileset.getTileUv(176, uv1, uvDiscard);
	//	Get the bottom right of the third tile
	guiTileset.getTileUv(178, uvDiscard, uv2);

	//	Scale end U by health percentage
	uv2.x *= healthPercent;

	glm::vec2 size = glm::vec2(48.0f * healthPercent, 16.0f);

	glm::vec2 tileSize = glm::vec2(16.0f, 16.0f);

	//	Draw three tiles for the bottom layer
	spriteBatch.buildTileVertexData(
		guiTileset,
		160,
		position * scale,
		tileSize * scale,
		false);

	spriteBatch.buildTileVertexData(
		guiTileset,
		161,
		position2 * scale,
		tileSize * scale,
		false);

	spriteBatch.buildTileVertexData(
		guiTileset,
		162,
		position3 * scale,
		tileSize * scale,
		false);

	//	Draw the filled layer
	spriteBatch.buildTileVertexData(
		guiTileset.texture.id,
		position * scale,
		size * scale,
		uv1,
		uv2,
		false);
}

//  ============================================================================
void renderPlayerHud(World& world, Camera& camera) {
	glm::mat4 projection = camera.getProjection();

	glDisable(GL_DEPTH_TEST);

	std::vector<Player*> players = world.getPlayers();

	if (players.size() > 0) {
		Player* player = players[0];
		HealthSystem& healthSystem = world.getHealthSystem();

		if (healthSystem.hasComponent(player->entity)) {
			HealthComponent healthCmpnt = healthSystem.getComponent(player->entity);
			HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);

			const float scale = 3.0f;
			glm::vec2 position = glm::vec2(4.0f, 4.0f);

			spriteBatch.begin();
			drawHealthGauge(position, scale, healthGauge.getValue(), healthGauge.getMax());
			spriteBatch.render(projection);
			spriteBatch.end();
		}
	}
}

//  ============================================================================
void renderSprites(
	SpriteSystem& spriteSystem,
	SpriteEffectSystem& spriteEffectSystem,
	Camera& camera) {
	glm::mat4 screenMvp = camera.getProjection() * camera.getView();

	glEnable(GL_DEPTH_TEST);

	spriteBatch.begin();
	spriteEffectSystem.buildVertexData(spriteBatch, spriteSystem);
	spriteBatch.render(screenMvp);
	spriteBatch.end();

	spriteBatch.begin();
	spriteSystem.buildVertexData(spriteBatch);
	spriteBatch.render(screenMvp);
	spriteBatch.end();
}
}