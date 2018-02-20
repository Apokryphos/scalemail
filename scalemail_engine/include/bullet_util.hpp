#include "light_data.hpp"
#include <glm/vec4.hpp>

namespace ScaleMail
{
	int getBulletImpactTilesetId(int bulletIndex);
	glm::vec4 getBulletLightColor(int bulletIndex);
	LightData getBulletLightData(int bulletIndex);
	int getBulletTilesetId(int bulletIndex);
}