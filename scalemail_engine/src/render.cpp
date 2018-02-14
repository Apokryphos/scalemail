#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
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
static Mesh quadMesh;
static QuadShader colorQuadShader;

//	============================================================================
void initializeRender(AssetManager& assetManager) {
	quadMesh = assetManager.getQuadMesh();
	colorQuadShader = assetManager.getColorQuadShader();

	initializeFont(assetManager);
	initializeTransition(assetManager);
	initializeLight(assetManager);
	initializeSprites(assetManager);
	initializeMapMesh(assetManager);
	initializeAmbientLights();
}

//	============================================================================
void updateStencilBuffer(GameWindow& gameWindow, Camera& camera) {
	Rectangle cameraBounds = camera.getBounds();

	glm::mat4 quadWorld =
		glm::translate(glm::vec3(
			cameraBounds.x + cameraBounds.width * 0.5f,
			cameraBounds.y + cameraBounds.height * 0.5f, 0.0f)) *
		glm::scale(glm::vec3(
			cameraBounds.width * 0.5f,
			cameraBounds.height * 0.5f, 1.0f));

	glm::mat4 screenProjection = glm::ortho(
		0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
		0.0f, 1.0f);

	glm::mat4 mvp = screenProjection * camera.getView() * quadWorld;

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
void render(Game& game, World& world, Camera& camera, GameState& gameState,
			float totalElapsedSeconds) {
	GameWindow& gameWindow = game.gameWindow;
	GLFWwindow* window = gameWindow.window;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwGetFramebufferSize(window, &gameWindow.width, &gameWindow.height);

	glViewport(0, 0, gameWindow.width, gameWindow.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateStencilBuffer(gameWindow, camera);

	renderMap(*world.getMap(), camera, totalElapsedSeconds);
	renderSprites(world.getSpriteSystem(), world.getSpriteEffectSystem(), camera);
	renderLight(gameWindow, camera, world.getLightSystem());
	renderTransition();

	if (game.drawCollision) {
		world.getPhysicsSystem().drawDebug(camera);
	}

	glDisable(GL_STENCIL_TEST);

	gameState.draw(game, camera);

	if (game.gui != nullptr) {
		game.gui->draw(world, camera);
	}

	renderText(gameWindow);

	glfwSwapBuffers(window);
}
}