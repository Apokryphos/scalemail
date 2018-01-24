#include "blend.hpp"
#include "ease.hpp"
#include "gl_headers.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "transition.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
#include <iostream>
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

static GLuint shader;
static GLuint shaderFadeColorLocation;
static GLuint shaderFadeProgressLocation;

static const struct
{
    float x, y;
} quadVertices[6] = {
    {  1.0f, -1.0f, },
    {  1.0f,  1.0f, },
    { -1.0f, -1.0f, },
    {  1.0f,  1.0f, },
    { -1.0f,  1.0f, },
    { -1.0f, -1.0f, }
};

//  ============================================================================
bool initFadeQuadMesh(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    mesh.vertexCount = 6;

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 2, (void*) 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

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
void initializeTransition() {
    initFadeQuadMesh(mesh);

    initShaderProgram("assets/shaders/fade.vert", "assets/shaders/fade.frag", shader);
    shaderFadeProgressLocation = glGetUniformLocation(shader, "fadeProgress");
    shaderFadeColorLocation = glGetUniformLocation(shader, "fadeColor");
}

//	============================================================================
void renderTransition() {
    glEnable(GL_BLEND);
    blendAlpha();

    float progress = transitionState == TransitionState::FADING_OUT ?
        easeInCubic(fadeTicks, 0, 1, fadeDuration) :
        easeOutCubic(fadeTicks, 0, 1, fadeDuration);

    glUseProgram(shader);
    glUniform1f(shaderFadeProgressLocation, progress);
    glUniform3fv(shaderFadeColorLocation, 1, &fadeColor[0]);

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);
}
}