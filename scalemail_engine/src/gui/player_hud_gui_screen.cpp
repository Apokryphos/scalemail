#include "gui/player_hud_gui_screen.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "health_system.hpp"
#include "inventory_system.hpp"
#include "item.hpp"
#include "player.hpp"
#include "sprite_batch.hpp"
#include "world.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
const int ITEM_SLOT_INDEX = 96;
const int ITEM_SLOT_EMPTY_INDEX = 112;

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

	const glm::vec2 tileSize =
		glm::vec2(guiTileset.getTileWidth(), guiTileset.getTileHeight());

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
		guiTileset.getTexture().id,
		position * scale,
		size * scale,
		uv1,
		uv2,
		false);
}

//  ============================================================================
static void drawItemSlot(SpriteBatch& spriteBatch, Tileset guiTileset,
						 glm::vec2 position, const float scale,
						 const Item* item, const Tileset& itemTileset) {
	//	Draw slot
	const glm::vec2 guiTileSize =
		glm::vec2(guiTileset.getTileWidth(), guiTileset.getTileHeight());

	glm::vec2 uv1;
	glm::vec2 uv2;

	const int slotTilesetId =
		item == nullptr ?
		ITEM_SLOT_EMPTY_INDEX :
		ITEM_SLOT_INDEX;

	guiTileset.getTileUv(slotTilesetId, uv1, uv2);

	spriteBatch.buildTileVertexData(
		guiTileset.getTexture().id,
		position * scale,
		guiTileSize * scale,
		uv1,
		uv2,
		false);

	if (item == nullptr) {
		return;
	}

	//	Draw item
	const glm::vec2 itemTileSize =
		glm::vec2(itemTileset.getTileWidth(), itemTileset.getTileHeight());

	itemTileset.getTileUv(item->tilesetId, uv1, uv2);

	spriteBatch.buildTileVertexData(
		itemTileset.getTexture().id,
		position * scale,
		itemTileSize * scale,
		uv1,
		uv2,
		true);
}

//	============================================================================
static void drawItemSlots(SpriteBatch& spriteBatch, Tileset guiTileset,
						 glm::vec2 position, const float scale,
						 const std::vector<std::shared_ptr<Item>>& items,
						 const Tileset& itemTileset) {
	const float xOffset = itemTileset.getTileWidth() + 2;

	//	Draw slot for each item
	for (const auto& item : items) {
		drawItemSlot(spriteBatch, guiTileset, position, scale, item.get(),
					 itemTileset);
		position.x += xOffset;
	}
}

//	============================================================================
void PlayerHudGuiScreen::draw(Game& game, SpriteBatch& spriteBatch) {
	World& world = *game.world;

	Camera* camera = game.camera;

	const float scale = std::min(camera->getZoom(), 3.0f);

	glm::mat4 projection = glm::ortho(
		0.0f,
		(float)game.gameWindow.getWidth(),
		(float)game.gameWindow.getHeight(),
		0.0f,
		-1.0f,
		1.0f);

	std::vector<Player*> players = world.getPlayers();

	if (players.size() > 0) {
		Player* player = players[0];
		HealthSystem& healthSystem = world.getHealthSystem();

		spriteBatch.begin();

		if (healthSystem.hasComponent(player->entity)) {
			HealthComponent healthCmpnt = healthSystem.getComponent(player->entity);
			HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);

			glm::vec2 position = glm::vec2(4.0f, 4.0f);

			drawHealthGauge(spriteBatch, mGuiTileset, position, scale,
							healthGauge.getValue(), healthGauge.getMax());
		}

		InventorySystem& inventorySystem = world.getInventorySystem();

		if (inventorySystem.hasComponent(player->entity)) {
			InventoryComponent inventoryCmpnt =
				inventorySystem.getComponent(player->entity);

			glm::vec2 position = glm::vec2(4.0f, 22.0f);

			const auto& items = inventorySystem.getItems(inventoryCmpnt);

			drawItemSlots(spriteBatch, mGuiTileset, position, scale, items,
						  mItemTileset);
		}

		spriteBatch.render(projection);
		spriteBatch.end();
	}
}

//	============================================================================
void PlayerHudGuiScreen::initialize(AssetManager& assetManager) {
	mGuiTileset = assetManager.getTileset("gui");
	mItemTileset = assetManager.getTileset("items");
}
}