#include "camera.hpp"
#include "ease.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
static const float transitionDuration = 3.5f;
static float transitionTicks = 0;

static glm::vec4 currentAmbientLightColor;
static glm::vec4 lastAmbientLightColor = glm::vec4(1.0f);
static glm::vec4 targetAmbientLightColor = glm::vec4(1.0f);

struct AmbientLight
{
	glm::vec4 color;
	glm::vec4 rect;
};

static std::vector<AmbientLight> ambientLights;

//  ============================================================================
void addAmbientLight(glm::vec4 color, glm::vec4 rect) {
	ambientLights.push_back({ color, rect });
}

//  ============================================================================
static bool rectContains(const glm::vec4 rect, const glm::vec2 point) {
	return
		point.x >= rect.x &&
		point.y >= rect.y &&
		point.x <= rect.x + rect.z &&
		point.y <= rect.y + rect.w;
}

//  ============================================================================
void updateAmbientLight(World& world, Camera& camera, float elapsedSeconds) {
	transitionTicks += elapsedSeconds;
	if (transitionTicks > transitionDuration) {
		transitionTicks = transitionDuration;
	}

	auto find = std::find_if(ambientLights.begin(), ambientLights.end(),
		[&camera](const AmbientLight& light) -> bool {
			return rectContains(light.rect, camera.position);
		});

	if (find != ambientLights.end()) {
		const AmbientLight& light = *find;

		if (targetAmbientLightColor != light.color) {
			lastAmbientLightColor = targetAmbientLightColor;
			targetAmbientLightColor = light.color;
			transitionTicks = 0;
		}
	}

	currentAmbientLightColor =
		easeVec4(transitionTicks, lastAmbientLightColor,
				 targetAmbientLightColor, transitionDuration, easeInOutCubic);

	world.getLightSystem().setAmbientColor(currentAmbientLightColor);
}
}