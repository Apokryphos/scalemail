#include <glm/vec4.hpp>

namespace ScaleMail
{
static const glm::vec4 BLUE_LIGHT_COLOR(0.46f, 0.76f, 0.97f, 1.0f);
static const glm::vec4 GREEN_LIGHT_COLOR(0.60f, 0.85f, 0.10f, 1.0f);
static const glm::vec4 ORANGE_LIGHT_COLOR(1.0f, 0.6f, 0.0f, 1.0f);
static const glm::vec4 VIOLET_LIGHT_COLOR(0.97f, 0.37f, 0.97f, 1.0f);

//  ============================================================================
int getBulletImpactTilesetId(int bulletIndex) {
	switch (bulletIndex) {
		case 0:
			return 8;
		case 1:
			return 24;
		case 2:
			return 40;
		case 3:
			return 56;
		default:
			return 0;
	}
}

//  ============================================================================
glm::vec4 getBulletLightColor(int bulletIndex) {
	switch (bulletIndex) {
		case 0:
			return ORANGE_LIGHT_COLOR;
		case 1:
			return BLUE_LIGHT_COLOR;
		case 2:
			return GREEN_LIGHT_COLOR;
		case 3:
			return VIOLET_LIGHT_COLOR;
		default:
			return ORANGE_LIGHT_COLOR;
	}
}

//  ============================================================================
int getBulletTilesetId(int bulletIndex) {
	switch (bulletIndex) {
		case 0:
			return 0;
		case 1:
			return 16;
		case 2:
			return 32;
		case 3:
			return 48;
		default:
			return 0;
	}
}
}