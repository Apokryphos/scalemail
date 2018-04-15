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
static const int ITEM_SLOT_INDEX = 96;
static const int ITEM_SLOT_EMPTY_INDEX = 112;
static const float PADDING = 1.0f;

//  ============================================================================
static void drawHealthGauge(SpriteBatch& spriteBatch, Tileset guiTileset,
							glm::vec2 position, const float scale,
							float healthValue, float healthMax) {
	const float healthPercent = healthValue / healthMax;

	const float tileWidth = guiTileset.getTileWidth();

	//	Positions for second and third tile for bottom (empty) layer
	const glm::vec2 position2 = position + glm::vec2(tileWidth * scale, 0.0f);
	const glm::vec2 position3 = position2 + glm::vec2(tileWidth * scale, 0.0f);

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

	//	Size of filled layer
	const glm::vec2 filledSize =
		glm::vec2(tileWidth * 3.0f * healthPercent, tileWidth);

	const glm::vec2 tileSize =
		glm::vec2(guiTileset.getTileWidth(), guiTileset.getTileHeight());

	//	Draw three tiles for the bottom layer
	spriteBatch.buildTileVertexData(
		guiTileset,
		160,
		position,
		tileSize * scale,
		false);

	spriteBatch.buildTileVertexData(
		guiTileset,
		161,
		position2,
		tileSize * scale,
		false);

	spriteBatch.buildTileVertexData(
		guiTileset,
		162,
		position3,
		tileSize * scale,
		false);

	//	Draw the filled layer
	spriteBatch.buildTileVertexData(
		guiTileset.getTexture().id,
		position,
		filledSize * scale,
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
		position,
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
		position,
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
	const float xOffset = (itemTileset.getTileWidth() + PADDING) * scale;

	//	Draw slot for each item
	for (const auto& item : items) {
		drawItemSlot(spriteBatch, guiTileset, position, scale, item.get(),
					 itemTileset);

		position.x += xOffset;
	}
}

//  ============================================================================
static void drawPortrait(SpriteBatch& spriteBatch, Tileset portraitTileset,
						 glm::vec2 position, const float scale,
						 const int actorIndex) {
	//	Draw slot
	const glm::vec2 tileSize =
		glm::vec2(portraitTileset.getTileWidth(), portraitTileset.getTileHeight());

	glm::vec2 uv1;
	glm::vec2 uv2;

	const int tilesetId = actorIndex * 10;

	portraitTileset.getTileUv(tilesetId, uv1, uv2);

	spriteBatch.buildTileVertexData(
		portraitTileset.getTexture().id,
		position,
		tileSize * scale,
		uv1,
		uv2,
		false);
}

//	============================================================================
void PlayerHudGuiScreen::draw(Game& game, SpriteBatch& spriteBatch) {
	World& world = *game.world;

	const Camera* camera = game.camera;

	const float scale = std::min(camera->getZoom(), 3.0f);

	const float screenWidth = (float)game.gameWindow.getWidth();
	const float screenHeight = (float)game.gameWindow.getHeight();

	const glm::mat4 projection = glm::ortho(
		0.0f,
		screenWidth,
		screenHeight,
		0.0f,
		-1.0f,
		1.0f);

	const float tileWidth = mGuiTileset.getTileWidth();
	const float tileHeight = mGuiTileset.getTileHeight();

	const float padding = PADDING * scale;
	const float itemSlotsWidth = tileWidth * 3.0f * scale;
	const float portraitWidth = mPortraitTileset.getTileWidth() * scale;

	const glm::vec2 positions[4] = {
		//	Top left
		{ padding, padding },
		//	Top right
		{ screenWidth - portraitWidth - padding,
		  padding },
		//	Bottom left
		{ padding,
		  screenHeight  - (tileHeight * 2.0f * scale) - padding},
		//	Bottom right
		{ screenWidth - portraitWidth - padding,
		  screenHeight - (tileHeight * 2.0f * scale) - padding },
	};

	const glm::vec2 offsets[4] = {
		//	Top left
		{ portraitWidth, 0.0f },
		//	Top right
		{ -itemSlotsWidth, 0.0f },
		//	Bottom left
		{ portraitWidth, 0.0f },
		//	Bottom right
		{ -itemSlotsWidth, 0.0f },
	};

	const std::vector<Player*> players = world.getPlayers();

	assert(players.size() <= 4);

	int p = 0;
	for (Player* player : players) {
		const HealthSystem& healthSystem = world.getHealthSystem();

		spriteBatch.begin();

		//	Draw portrait
		drawPortrait(spriteBatch, mPortraitTileset,
					 positions[p], scale,
					 player->actorIndex);

		//	Draw health gauge
		if (healthSystem.hasComponent(player->entity)) {
			const HealthComponent healthCmpnt =
				healthSystem.getComponent(player->entity);

			const HealthGauge& healthGauge =
				healthSystem.getHealthGauge(healthCmpnt);

			const glm::vec2 position = positions[p] + offsets[p];

			drawHealthGauge(spriteBatch, mGuiTileset, position, scale,
							healthGauge.getValue(), healthGauge.getMax());
		}

		//	Draw item slots
		const InventorySystem& inventorySystem = world.getInventorySystem();

		if (inventorySystem.hasComponent(player->entity)) {
			const InventoryComponent inventoryCmpnt =
				inventorySystem.getComponent(player->entity);

			glm::vec2 position = positions[p] + offsets[p];

			//	Adjust Y component so item slots are below health gauge
			position.y += (tileHeight * scale);

			//	Adjust X component of item slots on right side
			if (p == 1 || p == 3) {
				position.x -= padding * 2.0f;
			}

			const auto& items = inventorySystem.getItems(inventoryCmpnt);

			drawItemSlots(spriteBatch, mGuiTileset, position, scale, items,
						  mItemTileset);
		}

		spriteBatch.render(projection);
		spriteBatch.end();

		++p;
	}
}

//	============================================================================
void PlayerHudGuiScreen::initialize(AssetManager& assetManager) {
	mGuiTileset = assetManager.getTileset("gui");
	mItemTileset = assetManager.getTileset("items");
	mPortraitTileset= assetManager.getTileset("portraits", 32, 32);
}
}