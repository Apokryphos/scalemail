#pragma once

#include "ambient_light.hpp"
#include "direction.hpp"
#include "map_mesh.hpp"
#include "map_model.hpp"
#include "mesh.hpp"
#include "rectangle.hpp"
#include <glm/vec2.hpp>
#include <string>
#include <vector>

namespace ScaleMail
{
struct PlayerStart
{
	int actorIndex;
	Direction facing;
	glm::vec2 position;
};

class Map
{
	int mWidth;
	int mHeight;
	int mTileWidth;
	int mTileHeight;
	MapModel mMapModel;

	std::vector<PlayerStart> mPlayerStarts;
	std::vector<AmbientLight> mAmbientLights;

public:
	Map(const int width, const int height, const int tileWidth,
		const int tileHeight, const MapModel mapModel);
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	const std::vector<AmbientLight>& getAmbientLights() const;
	int getHeight() const;
	std::vector<PlayerStart> getPlayerStarts() const;
	int getTileHeight() const;
	int getTileWidth() const;
	int getWidth() const;
	const MapModel& getMapModel() const;
	bool pointInBounds(const glm::vec2& point);
	void setAmbientLights(std::vector<AmbientLight>& ambientLights);
	void setPlayerStarts(const std::vector<PlayerStart> playerStarts);
	bool tileInBounds(const int tileX, int tileY);
};
}