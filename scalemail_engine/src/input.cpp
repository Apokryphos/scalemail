#include "game.hpp"
#include "gl_headers.hpp"
#include "input_state.hpp"

namespace ScaleMail
{
struct InputDevice
{
	InputState inputState;
};

InputDevice keyboard;

//  ============================================================================
InputState getKeyboardInputState() {
	return keyboard.inputState;
}

//  ============================================================================
static void keyCallback(GLFWwindow* window, int key,
						[[maybe_unused]] int scancode, int action,
						[[maybe_unused]] int mods) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	InputState& inputState = keyboard.inputState;

	//  Single keypress
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				game->quit = true;
				break;

			case GLFW_KEY_F1:
				inputState.capture = !inputState.capture;
				break;

			case GLFW_KEY_P:
				game->paused = !game->paused;
				break;
		}
	}

	bool value = !(action == GLFW_RELEASE);

	switch (key) {
		case GLFW_KEY_DOWN:
			inputState.moveDown = value;
			break;

		case GLFW_KEY_LEFT:
			inputState.moveLeft = value;
			break;

		case GLFW_KEY_RIGHT:
			inputState.moveRight = value;
			break;

		case GLFW_KEY_UP:
			inputState.moveUp = value;
			break;
	}

	//  Set first player to keyboard input state
	auto players = game->world->getPlayers();
	if (players.size() > 0) {
		players[0]->inputState = inputState;
	}
}

//  ============================================================================
void initializeInput(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
}
}