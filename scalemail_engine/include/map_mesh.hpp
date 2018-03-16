#pragma once

#include "gl_headers.hpp"
#include "mesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace ScaleMail
{
struct MapMesh
{
	bool alpha;
	bool animated;
	bool scroll;
	int frame;
	GLuint textureId;
	Mesh mesh;
	std::vector<float> vertexData;
};

void addTileVertexData(std::vector<float>& meshVertexData,
					   const glm::vec3 position, const glm::vec3 size,
					   const glm::vec2 uv1, const glm::vec2 uv2,
					   bool flipDiag, bool flipHorz, bool flipVert);
}