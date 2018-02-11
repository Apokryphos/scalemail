#include "map.hpp"

namespace ScaleMail
{
//  ============================================================================
Map::Map(const int width, const int height) :
	height(height),
	width(width) {
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
