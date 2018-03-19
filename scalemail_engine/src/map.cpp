#include "map.hpp"

namespace ScaleMail
{
//  ============================================================================
Map::Map(const int width, const int height, const int tileWidth,
		 const int tileHeight, const MapModel mapModel) :
	mWidth(width), mHeight(height), mTileWidth(tileWidth),
	mTileHeight(tileHeight), mMapModel(mapModel) {
}

//  ============================================================================
const std::vector<AmbientLight>& Map::getAmbientLights() const {
	return mAmbientLights;
}

//  ============================================================================
int Map::getHeight() const {
	return mHeight;
}

//  ============================================================================
const MapModel& Map::getMapModel() const {
	return mMapModel;
}

//  ============================================================================
std::vector<PlayerStart> Map::getPlayerStarts() const {
	return mPlayerStarts;
}

//  ============================================================================
int Map::getTileHeight() const {
	return mTileHeight;
}

//  ============================================================================
int Map::getTileWidth() const {
	return mTileWidth;
}

//  ============================================================================
int Map::getWidth() const {
	return mWidth;
}

//  ============================================================================
bool Map::pointInBounds(const glm::vec2& point) {
	return
		point.x >= 0 &&
		point.y >= 0 &&
		point.x <= mWidth * mTileWidth &&
		point.y <= mHeight * mTileHeight;
}

//  ============================================================================
void Map::setAmbientLights(std::vector<AmbientLight>& ambientLights) {
	mAmbientLights = ambientLights;
}

//  ============================================================================
void Map::setPlayerStarts(const std::vector<PlayerStart> playerStarts) {
	mPlayerStarts = playerStarts;
}

//  ============================================================================
bool Map::tileInBounds(const int tileX, int tileY) {
	return
		tileX >= 0 &&
		tileY >= 0 &&
		tileX <= mWidth &&
		tileY <= mHeight;
}
}
