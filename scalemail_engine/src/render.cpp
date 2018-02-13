#include "camera.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "map_render.hpp"
#include "light.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
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

	renderMap(*world.getMap(), camera, totalElapsedSeconds);
	renderSprites(world.getSpriteSystem(), world.getSpriteEffectSystem(), camera);
	renderLight(gameWindow, camera, world.getLightSystem());
	renderTransition();

	if (game.drawCollision) {
		world.getPhysicsSystem().drawDebug(camera);
	}

	gameState.draw(game, camera);

	renderPlayerHud(world, camera);

	renderText(gameWindow);

	glfwSwapBuffers(window);
}
}