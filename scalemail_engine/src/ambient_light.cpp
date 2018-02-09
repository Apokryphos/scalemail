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
static AmbientLight* getAmbientLight(const Camera& camera) {
	auto find = std::find_if(ambientLights.begin(), ambientLights.end(),
		[&camera](const AmbientLight& light) -> bool {
			return rectContains(light.rect, camera.position);
		});

	if (find != ambientLights.end()) {
		return &(*find);
	}

	return nullptr;
}

//  ============================================================================
void initializeAmbientLight(World& world, Camera& camera) {
	const AmbientLight* light = getAmbientLight(camera);

	if (light != nullptr) {
		lastAmbientLightColor = light->color;
		targetAmbientLightColor = light->color;
		world.getLightSystem().setAmbientColor(light->color);
	}
}

//  ============================================================================
void updateAmbientLight(World& world, Camera& camera, float elapsedSeconds) {
	transitionTicks += elapsedSeconds;
	if (transitionTicks > transitionDuration) {
		transitionTicks = transitionDuration;
	}

	const AmbientLight* light = getAmbientLight(camera);

	if (light != nullptr) {
		if (targetAmbientLightColor != light->color) {
			lastAmbientLightColor = targetAmbientLightColor;
			targetAmbientLightColor = light->color;
			transitionTicks = 0;
		}
	}

	if (targetAmbientLightColor != lastAmbientLightColor) {
		currentAmbientLightColor =
			easeVec4(transitionTicks, lastAmbientLightColor,
					targetAmbientLightColor, transitionDuration, easeInOutCubic);
	}
	else {
		currentAmbientLightColor = targetAmbientLightColor;
	}

	world.getLightSystem().setAmbientColor(currentAmbientLightColor);
}
}