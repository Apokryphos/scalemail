#pragma once

#include "direction.hpp"
#include "mesh.hpp"
#include "rectangle.hpp"
#include <glm/vec2.hpp>
#include <string>

namespace ScaleMail
{
struct MapMesh
{
	Mesh staticMesh;
	Mesh alphaMesh;
	Mesh animatedMeshes[2];
	Mesh scrollMeshes[2];
};

struct PlayerStart
{
	int actorIndex;
	Direction facing;
	glm::vec2 position;
};

struct MapCameraPath {
	std::vector<glm::vec2> points;
};

struct MapCamera {
	std::string name;
	std::vector<Rectangle> bounds;
	std::vector<MapCameraPath> paths;
};

class Map
{
	int mWidth;
	int mHeight;
	int mTileWidth;
	int mTileHeight;

	std::vector<MapCamera> mCameras;
	std::vector<PlayerStart> mPlayerStarts;

public:
	Map(const int width, const int height, const int tileWidth,
		const int tileHeight);
	void addCamera(const MapCamera& mapCamera);
	const MapCamera* getCamera(const std::string name) const;
	int getHeight() const;
	std::vector<PlayerStart> getPlayerStarts() const;
	int getTileHeight() const;
	int getTileWidth() const;
	int getWidth() const;
	MapMesh mapMesh;
	void setPlayerStarts(const std::vector<PlayerStart> playerStarts);
};
}