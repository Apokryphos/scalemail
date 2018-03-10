#include "asset_manager.hpp"
#include "blend.hpp"
#include "ease.hpp"
#include "fade_shader.hpp"
#include "gl_headers.hpp"
#include "mesh.hpp"
#include "transition.hpp"
#include <glm/glm.hpp>
#include <algorithm>
#include <string>
#include <vector>

namespace ScaleMail
{
enum class FadeDirection {
	NONE,
	FADE_IN,
	FADE_OUT,
};

static TransitionState transitionState;

static FadeDirection fadeDirection = FadeDirection::NONE;
static float fadeTicks = 0.0f;
static float fadeDuration = 3.0f;

static glm::vec3 fadeColor = glm::vec3(0.0f, 0.0f, 0.0f);

static Mesh mesh;

static FadeShader fadeShader;

static const std::vector<float> FADE_QUAD_VERTEX_DATA = {
	 1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
	-1.0f, -1.0f
};

//	============================================================================
void transitionFadeIn() {
	fadeTicks = fadeDuration;
	fadeDirection = FadeDirection::FADE_IN;
}

//	============================================================================
void transitionFadeOut() {
	fadeTicks = 0;
	fadeDirection = FadeDirection::FADE_OUT;
}

//	============================================================================
void setTransitionDuration(float duration) {
	fadeDuration = duration;
}

//	============================================================================
void setTransitionState(TransitionState state)
{
	switch (state) {
		case TransitionState::NONE:
			fadeTicks = 0;
			transitionState = state;
			fadeDirection = FadeDirection::NONE;
			break;

		case TransitionState::FADING_OUT:
			fadeTicks = 0;
			transitionState = state;
			fadeDirection = FadeDirection::FADE_OUT;
			break;

		case TransitionState::FADED_OUT:
			fadeTicks = fadeDuration;
			transitionState = state;
			fadeDirection = FadeDirection::NONE;
			break;

		case TransitionState::FADING_IN:
			fadeTicks = fadeDuration;
			transitionState = state;
			fadeDirection = FadeDirection::FADE_IN;
			break;
	}
}

//	============================================================================
void addTransitionTime(float elapsedSeconds) {
	if (fadeDirection == FadeDirection::FADE_IN) {
		fadeTicks = std::max(fadeTicks - elapsedSeconds, 0.0f);
	} else if (fadeDirection == FadeDirection::FADE_OUT) {
		fadeTicks = std::min(fadeTicks + elapsedSeconds, fadeDuration);
	}
}

//	============================================================================
void initializeTransition(AssetManager& assetManager,
						  const RenderOptions& renderOptions) {
	fadeShader = assetManager.getFadeShader();

	initMesh(mesh, VertexDefinition::POSITION2, renderOptions);
	setMeshVertexData(mesh, FADE_QUAD_VERTEX_DATA);
}

//	============================================================================
void renderTransition() {
	glEnable(GL_BLEND);
	blendAlpha();

	float progress = transitionState == TransitionState::FADING_OUT ?
		easeInCubic(fadeTicks, 0, 1, fadeDuration) :
		easeOutCubic(fadeTicks, 0, 1, fadeDuration);

	glUseProgram(fadeShader.id);
	glUniform1f(fadeShader.fadeProgressLocation, progress);
	glUniform3fv(fadeShader.fadeColorLocation, 1, &fadeColor[0]);

	drawMesh(mesh);
}
}