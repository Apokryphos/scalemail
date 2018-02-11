#pragma once

#include "direction.hpp"
#include "mesh.hpp"
#include <glm/glm.hpp>

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
	std::vector<PlayerStart> mPlayerStarts;

public:
	Map(const int width, const int height);
	std::vector<PlayerStart> getPlayerStarts() const;
	const int height;
	MapMesh mapMesh;
	void setPlayerStarts(const std::vector<PlayerStart> playerStarts);
	const int width;
};
}