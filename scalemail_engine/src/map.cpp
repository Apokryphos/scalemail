#include "map.hpp"

namespace ScaleMail
{
//  ============================================================================
Map::Map(const int width, const int height) :
	height(height),
	width(width) {
}

//  ============================================================================
std::vector<Rectangle> Map::getCameraBounds() const {
	return mCameraBounds;
}

//  ============================================================================
std::vector<PlayerStart> Map::getPlayerStarts() const {
	return mPlayerStarts;
}

//  ============================================================================
void Map::setCameraBounds(const std::vector<Rectangle> cameraBounds) {
	mCameraBounds = cameraBounds;
}

//  ============================================================================
void Map::setPlayerStarts(const std::vector<PlayerStart> playerStarts) {
	mPlayerStarts = playerStarts;
}
}
