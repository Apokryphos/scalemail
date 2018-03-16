#include "map_model.hpp"

namespace ScaleMail
{
//  ============================================================================
MapModel::MapModel(std::vector<MapMesh>& mapMeshes) : mMapMeshes(mapMeshes) {
}

//  ============================================================================
const std::vector<MapMesh>& MapModel::getMapMeshes() const {
	return mMapMeshes;
}
}