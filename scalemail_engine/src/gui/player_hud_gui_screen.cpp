#include "gui/player_hud_gui_screen.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "sprite_batch.hpp"
#include "world.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
//  ============================================================================
static void drawHealthGauge(SpriteBatch& spriteBatch, Tileset guiTileset,
							glm::vec2 position, const float scale,
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

//	============================================================================
void PlayerHudGuiScreen::draw(Game& game, SpriteBatch& spriteBatch) {
	Camera& camera = *game.camera;
	World& world = *game.world;

	glm::mat4 projection = glm::ortho(
		0.0f,
		(float)game.gameWindow.width,
		(float)game.gameWindow.height,
		0.0f,
		-1.0f,
		1.0f);

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
			drawHealthGauge(spriteBatch, mGuiTileset, position, scale,
							healthGauge.getValue(), healthGauge.getMax());
			spriteBatch.render(projection);
			spriteBatch.end();
		}
	}
}

//	============================================================================
void PlayerHudGuiScreen::initialize(AssetManager& assetManager) {
	mGuiTileset = assetManager.getTileset("gui");
}
}