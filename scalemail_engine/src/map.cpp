#include "map.hpp"

namespace ScaleMail
{
//  ============================================================================
Map::Map(const int width, const int height) :
	height(height),
	width(width) {
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
std::vector<PlayerStart> Map::getPlayerStarts() const {
	return mPlayerStarts;
}

//  ============================================================================
void Map::setPlayerStarts(const std::vector<PlayerStart> playerStarts) {
	mPlayerStarts = playerStarts;
}
}
