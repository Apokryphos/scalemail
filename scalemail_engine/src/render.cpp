#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "map_render.hpp"
#include "mesh.hpp"
#include "light.hpp"
#include "sprite.hpp"
#include "quad_shader.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
static Mesh debugLineMesh;
static std::vector<float> debugLineVertexData;
static LineShader debugLineShader;

static Mesh quadMesh;
static QuadShader colorQuadShader;
static QuadShader particleShader;

//	============================================================================
void initializeRender(AssetManager& assetManager) {
	quadMesh = assetManager.getQuadMesh();
	colorQuadShader = assetManager.getColorQuadShader();
	particleShader = assetManager.getParticleShader();

	debugLineShader = assetManager.getLineShader();
	initLineMesh(debugLineMesh, {});
	debugLineVertexData.reserve(500000);

	initializeFont(assetManager);
	initializeTransition(assetManager);
	initializeLight(assetManager);
	initializeSprites(assetManager);
	initializeMapMesh(assetManager);
	initializeAmbientLights();
}

//	============================================================================
void updateStencilBuffer(Camera& camera) {
	Rectangle cameraBounds = camera.getBounds();

	glm::mat4 quadWorld =
		glm::translate(glm::vec3(
			cameraBounds.x + cameraBounds.width * 0.5f,
			cameraBounds.y + cameraBounds.height * 0.5f, 0.0f)) *
		glm::scale(glm::vec3(
			cameraBounds.width * 0.5f,
			cameraBounds.height * 0.5f, 1.0f));

	const glm::mat4 mvp =
		camera.getProjection() * camera.getView() * quadWorld;

	//	Prepare stencil buffer writes
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	//	Draw camera bounds quad
	glUseProgram(colorQuadShader.id);
	glUniformMatrix4fv(colorQuadShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(quadMesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, quadMesh.vertexCount);

	//	Disable writes stencil buffer
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
}

//	============================================================================
void renderDebug(Game& game, Camera& camera) {
	//	Clear vertex data
	debugLineVertexData.resize(0);

	if (game.devOptions.enabled) {
		World& world = *game.world;

		if (game.devOptions.drawCollision) {
			world.getPhysicsSystem().drawDebug(debugLineVertexData);
		}

		if (game.devOptions.drawAi) {
			world.getAiSystem().drawDebug(debugLineVertexData);
		}

		if (game.devOptions.drawTriggers) {
			world.getTriggerSystem().drawDebug(debugLineVertexData);
		}

		if (debugLineVertexData.size() > 0) {
			const glm::mat4 mvp = camera.getProjection() * camera.getView();

			updateMesh(debugLineMesh, debugLineVertexData);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(debugLineShader.id);
			glUniformMatrix4fv(debugLineShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
			glBindVertexArray(debugLineMesh.vao);
			glDrawArrays(GL_LINES, 0, debugLineMesh.vertexCount);
		}
	}
}

//	============================================================================
void renderParticles(World& world, Camera& camera) {
	ParticleSystem& particleSystem = world.getParticleSystem();

	particleSystem.buildVertexData();

	const Mesh& mesh = particleSystem.getMesh();

	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	glUseProgram(particleShader.id);
	glUniformMatrix4fv(particleShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
}

//	============================================================================
void render(Game& game, World& world, Camera& camera, GameState& gameState,
			float totalElapsedSeconds) {
	GameWindow& gameWindow = game.gameWindow;
	GLFWwindow* window = gameWindow.window;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwGetFramebufferSize(window, &gameWindow.width, &gameWindow.height);

	glViewport(0, 0, gameWindow.width, gameWindow.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	Don't use stencil buffer if the camera is in 3D
	if (!game.devOptions.camera3d) {
		updateStencilBuffer(camera);
	}

	renderMap(*world.getMap(), camera, totalElapsedSeconds);
	renderSprites(world.getSpriteSystem(), world.getSpriteEffectSystem(), camera);
	renderLight(gameWindow, camera, world.getLightSystem());

	glEnable(GL_DEPTH_TEST);
	blendAlphaAdditive();
	renderParticles(world, camera);
	blendNone();
	glDisable(GL_DEPTH_TEST);

	renderTransition();

	//	Debug drawing
	renderDebug(game, camera);

	glDisable(GL_STENCIL_TEST);

	gameState.draw(game, camera);

	//	Draw GUI
	if (!game.devOptions.enabled || !game.devOptions.hideGui) {
		if (game.gui != nullptr) {
			game.gui->draw(game);
		}

		renderText(gameWindow);
	}

	glfwSwapBuffers(window);
}
}