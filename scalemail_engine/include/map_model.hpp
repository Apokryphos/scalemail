#pragma once

#include "map_mesh.hpp"
#include <vector>

namespace ScaleMail
{

class MapModel
{
	std::vector<MapMesh> mMapMeshes;

public:
	MapModel(std::vector<MapMesh>& mapMeshes);
	const std::vector<MapMesh>& getMapMeshes() const;
};
}