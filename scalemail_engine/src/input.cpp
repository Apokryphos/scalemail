#include "game.hpp"
#include "gl_headers.hpp"
#include "input_state.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <algorithm>
#include <iostream>

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
static void updatePlayerInputState(World& world) {
	//  Set first player to keyboard input state
	auto players = world.getPlayers();
	if (players.size() > 0) {
		players[0]->inputState = keyboard.inputState;
	}
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
				game->drawCollision = !game->drawCollision;
				break;

			case GLFW_KEY_F10:
				inputState.capture = !inputState.capture;
				break;

			case GLFW_KEY_D:
				game->devMode = !game->devMode;
				break;

			case GLFW_KEY_O:
				transitionFadeOut();
				break;

			case GLFW_KEY_F:
				game->cameraFollow = !game->cameraFollow;
				break;

			case GLFW_KEY_I:
				transitionFadeIn();
				break;

			case GLFW_KEY_P:
				game->paused = !game->paused;
				break;

			case GLFW_KEY_MINUS:
				game->speed = std::max(game->speed - 0.5, 1.0);
				std::cout << "Game speed decreased to "
						  << game->speed << "." << std::endl;
				break;

			case GLFW_KEY_EQUAL:
				game->speed += 0.5;
				std::cout << "Game speed increased to "
						  << game->speed << "." << std::endl;
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

	updatePlayerInputState(*game->world);
}

//  ============================================================================
static void mouseButtonCallback(GLFWwindow* window, int button, int action,
								[[maybe_unused]] int mods) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	World* world = game->world;

	InputState& inputState = keyboard.inputState;

	if (world == nullptr) {
		inputState.fire = false;
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			inputState.fire = true;
		} else {
			inputState.fire = false;
		}
	}

	updatePlayerInputState(*world);
}

//  ============================================================================
static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
	//	Update aim reticle position
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	keyboard.inputState.aimPosition = glm::vec2(xpos, ypos);
	updatePlayerInputState(*game->world);
}

//  ============================================================================
void initializeInput(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePositionCallback);
}
}