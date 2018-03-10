#pragma once

#include "ambient_light.hpp"
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
	std::vector<AmbientLight> mAmbientLights;

public:
	Map(const int width, const int height, const int tileWidth,
		const int tileHeight);
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	void addCamera(const MapCamera& mapCamera);
	const std::vector<AmbientLight>& getAmbientLights() const;
	const MapCamera* getCamera(const std::string name) const;
	int getHeight() const;
	std::vector<PlayerStart> getPlayerStarts() const;
	int getTileHeight() const;
	int getTileWidth() const;
	int getWidth() const;
	MapMesh mapMesh;
	bool pointInBounds(const glm::vec2& point);
	void setAmbientLights(std::vector<AmbientLight>& ambientLights);
	void setPlayerStarts(const std::vector<PlayerStart> playerStarts);
	bool tileInBounds(const int tileX, int tileY);
};
}