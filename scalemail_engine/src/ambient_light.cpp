#include "ambient_light.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "render_options.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
static Mesh ambientLightMesh;
static std::vector<float> ambientLightVertexData;

//  ============================================================================
void initializeAmbientLights(const RenderOptions& renderOptions) {
	initMesh(ambientLightMesh, VertexDefinition::POSITION2_COLOR4,
			 renderOptions);
}

//  ============================================================================
void buildAmbientLights(const std::vector<AmbientLight>& lights) {
	ambientLightVertexData.resize(0);

	for (const auto& light : lights) {
		glm::vec2 position = glm::vec2(light.rect.x, light.rect.y);
		glm::vec2 size = glm::vec2(light.rect.z, light.rect.w);

		addQuadVertexData(ambientLightVertexData, position, size, light.color);
	}

	setMeshVertexData(ambientLightMesh, ambientLightVertexData);
}

//  ============================================================================
void drawAmbientLights() {
	drawMesh(ambientLightMesh);
}
}