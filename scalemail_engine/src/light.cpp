#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light_system.hpp"
#include "math_util.hpp"
#include "mesh.hpp"
#include "quad_shader.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
static const GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

static int fboSize = 1024;
static GLuint fboA;
static GLuint fboATexture;
static GLuint fboB;
static GLuint fboBTexture;

static Texture lightTexture;

static SpriteBatch lightSpriteBatch;
static SpriteBatch glowSpriteBatch;

static Mesh quadMesh;
static QuadShader quadShader;
static QuadShader colorQuadShader;

//  ============================================================================
static void createFramebuffer(GLuint& fbo, int size, GLuint& fboTexture) {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);
	glDrawBuffers(1, drawBuffers);
}

//  ============================================================================
static void destroyFramebuffer(GLuint& fbo, GLuint& fboTexture) {
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &fboTexture);
}

//  ============================================================================
void initializeLight(AssetManager& assetManager) {
	quadShader = assetManager.getQuadShader();
	colorQuadShader = assetManager.getColorQuadShader();

	quadMesh = assetManager.getQuadMesh();

	lightSpriteBatch.initialize("Light", assetManager);
	glowSpriteBatch.initialize("Glow", assetManager);

	lightTexture = assetManager.loadTexture("light");

	createFramebuffer(fboA, fboSize, fboATexture);
	createFramebuffer(fboB, fboSize, fboBTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//  ============================================================================
int getMinFramebufferSize(GLFWwindow* window) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	int max = std::max(width, height);

	return isPowerOfTwo(max) ? max : nextPowerOfTwo(max);
}

//  ============================================================================
void renderLight(GameWindow& gameWindow, Camera& camera,
				 LightSystem& lightSystem) {
	const int minFrameBufferSize =
		getMinFramebufferSize(gameWindow.getGlfwWindow());

	if (fboSize != minFrameBufferSize) {
		fboSize = minFrameBufferSize;
		destroyFramebuffer(fboA, fboATexture);
		destroyFramebuffer(fboB, fboBTexture);
		createFramebuffer(fboA, fboSize, fboATexture);
		createFramebuffer(fboB, fboSize, fboBTexture);
		std::cout << "Light framebuffers resized to " << fboSize << std::endl;
	}

	const glm::mat4 quadWorld =
		glm::translate(glm::vec3(fboSize * 0.5f, fboSize * 0.5f, 0.0f)) *
		glm::scale(glm::vec3(fboSize * 0.5f, fboSize * 0.5f, 1.0f));

	const glm::mat4 fboProjection = glm::ortho(
		0.0f, (float)fboSize / camera.getZoom(),
		0.0f, (float)fboSize / camera.getZoom(),
		0.0f, 1.0f);

	const glm::mat4 screenProjection = glm::ortho(
		0.0f, (float)gameWindow.getWidth(),
		(float)gameWindow.getHeight(), 0.0f,
		0.0f, 1.0f);

	const glm::mat4 lightMvp = fboProjection * camera.getTransform();

	const glm::mat4 screenMvp = screenProjection * quadWorld;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//  Draw lights to FBO A
	glowSpriteBatch.begin();
	lightSystem.buildGlowVertexData(glowSpriteBatch);
	blendAdditive();
	glBindFramebuffer(GL_FRAMEBUFFER, fboA);
	glViewport(0, 0, fboSize, fboSize);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, lightTexture.id);
	glowSpriteBatch.render(lightMvp);
	glowSpriteBatch.end();

	//	Draw ambient light color quads to FBO B
	blendNone();
	glBindFramebuffer(GL_FRAMEBUFFER, fboB);
	glViewport(0, 0, fboSize, fboSize);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(colorQuadShader.id);
	glUniformMatrix4fv(colorQuadShader.mvpLocation, 1, GL_FALSE, &lightMvp[0][0]);
	drawAmbientLights();

	//  Draw lights to FBO B
	lightSpriteBatch.begin();
	lightSystem.buildVertexData(lightSpriteBatch);
	blendAdditive();
	lightSpriteBatch.render(lightMvp);
	lightSpriteBatch.end();

	//  Draw lights from FBO A to screen
	blendAdditive();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, gameWindow.getWidth(), gameWindow.getHeight());
	glUseProgram(quadShader.id);
	glUniformMatrix4fv(quadShader.mvpLocation, 1, GL_FALSE, &screenMvp[0][0]);
	glBindTexture(GL_TEXTURE_2D, fboATexture);

	enableMeshVertexAttribPointers(quadMesh);
	glDrawArrays(quadMesh.primitive, 0, quadMesh.vertexCount);

	//  Draw lights from FBO B to screen
	blendModulate();
	glBindTexture(GL_TEXTURE_2D, fboBTexture);
	glDrawArrays(quadMesh.primitive, 0, quadMesh.vertexCount);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	disableMeshVertexAttribPointers(quadMesh);
}
}