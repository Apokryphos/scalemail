#include "map.hpp"

namespace ScaleMail
{
//  ============================================================================
Map::Map(const int width, const int height, const int tileWidth,
		 const int tileHeight) :
	mWidth(width), mHeight(height), mTileWidth(tileWidth),
	mTileHeight(tileHeight) {
}

//  ============================================================================
void Map::addCamera(const MapCamera& mapCamera) {
	mCameras.push_back(mapCamera);
}

//  ============================================================================
const MapCamera* Map::getCamera(const std::string name) const {
	for (const MapCamera& camera : mCameras) {
		if (camera.name == name) {
			return &camera;
		}
	}

	return nullptr;
}

//  ============================================================================
int Map::getHeight() const {
	return mHeight;
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
void Map::setPlayerStarts(const std::vector<PlayerStart> playerStarts) {
	mPlayerStarts = playerStarts;
}
}
