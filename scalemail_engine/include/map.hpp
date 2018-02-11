#pragma once

#include "direction.hpp"
#include "mesh.hpp"
#include "rectangle.hpp"
#include <glm/vec2.hpp>

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

class Map
{
	std::vector<Rectangle> mCameraBounds;
	std::vector<PlayerStart> mPlayerStarts;

public:
	Map(const int width, const int height);
	std::vector<Rectangle> getCameraBounds() const;
	std::vector<PlayerStart> getPlayerStarts() const;
	const int height;
	MapMesh mapMesh;
	void setCameraBounds(const std::vector<Rectangle> cameraBounds);
	void setPlayerStarts(const std::vector<PlayerStart> playerStarts);
	const int width;
};
}