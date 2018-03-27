#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "mesh.hpp"
#include "vertex_data.hpp"
#include <vector>

namespace ScaleMail
{
static Mesh ambientLightMesh;
static std::vector<float> ambientLightVertexData;

//  ============================================================================
void initializeAmbientLights(const AssetManager& assetManager) {
	assetManager.initializeMesh(ambientLightMesh,
								VertexDefinition::POSITION2_COLOR4);
}

//  ============================================================================
void buildAmbientLights(const std::vector<AmbientLight>& lights) {
	ambientLightVertexData.resize(0);

	for (const auto& light : lights) {
		const auto& indices = light.polygon.getIndices();
		const auto& points = light.polygon.getPoints();

		for (const auto& n : indices) {
			const auto& point = points[n];

			ambientLightVertexData.emplace_back(point.x);
			ambientLightVertexData.emplace_back(point.y);
			ambientLightVertexData.emplace_back(light.color.r);
			ambientLightVertexData.emplace_back(light.color.g);
			ambientLightVertexData.emplace_back(light.color.b);
			ambientLightVertexData.emplace_back(light.color.a);
		}
	}

	setMeshVertexData(ambientLightMesh, ambientLightVertexData);
}

//  ============================================================================
void drawAmbientLights() {
	drawMesh(ambientLightMesh);
}
}