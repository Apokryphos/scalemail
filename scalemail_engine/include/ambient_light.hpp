#pragma once

#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
struct AssetManager;

struct AmbientLight
{
	glm::vec4 color;
	glm::vec4 rect;
};

void buildAmbientLights(const std::vector<AmbientLight>& lights);
void drawAmbientLights();
void initializeAmbientLights(const AssetManager& assetManager);
}