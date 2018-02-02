#pragma once

#include "mesh.hpp"

namespace ScaleMail
{
struct MapMesh {
	Mesh staticMesh;
	Mesh alphaMesh;
	Mesh animatedMeshes[2];
	Mesh scrollMeshes[2];
};

class Map
{
public:
	Map(const int width, const int height);
	const int height;
	MapMesh mapMesh;
	const int width;
};
}