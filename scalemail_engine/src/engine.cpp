#include "asset_manager.hpp"
#include "camera.hpp"
#include "cursor.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "gl_headers.hpp"
#include "input.hpp"
#include "input_state.hpp"
#include "light.hpp"
#include "map_render.hpp"
#include "render.hpp"
#include "screen_capture.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <algorithm>
#include <iostream>

namespace ScaleMail {

const int CAPTURE_SKIP_FRAMES = 2;
int captureSkipFrames = 0;
ScreenCapture capture;

//  ============================================================================
static void errorCallback(int error, const char* description) {
	std::cerr << "GL Error " << error << ": " << description << std::endl;
}

//  ============================================================================
static void screenCapture() {
	InputState inputState = getKeyboardInputState();

	if (inputState.capture) {
		capture.startCapture();
		inputState.capture = false;
	} else {
		if (capture.isCapturing()) {
			capture.stopCapture();
		}
	}

	if (capture.isCapturing()) {
		if (captureSkipFrames > 0) {
			--captureSkipFrames;
		} else {
			capture.captureFrame();
			captureSkipFrames = CAPTURE_SKIP_FRAMES;
		}
	}
}

//  ============================================================================
int startEngine() {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const int screenWidth = 1024;
	const int screenHeight = 1024;
	const float cameraZoom = 4.0f;

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "ScaleMail",
										  NULL, NULL);

	if (!window) {
		std::cerr << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeLimits(window, screenWidth, screenHeight,
							screenWidth, screenHeight);

	capture.initialize(window);

	initializeInput(window);

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		std::cerr << "Glad failed to initialize." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(0);
	glEnable(GL_FRAMEBUFFER_SRGB);

	loadCursor(window);

	AssetManager assetManager;
	assetManager.initialize();

	initializeFont(assetManager);
	initializeTransition(assetManager);
	initializeLight(assetManager);
	initializeSprites(assetManager);
	initializeMapMesh(assetManager);

	//  Load map after all other initialize functions
	World world;
	world.initialize(&assetManager);

	world.loadMap("map1");

	Camera camera(screenWidth, screenHeight, cameraZoom);

	Game game = {};
	game.camera = &camera;
	game.gameWindow.window = window;
	game.speed = 1.0;
	game.world = &world;
	glfwSetWindowUserPointer(window, &game);

	GameStateManager gameStateManager;
	gameStateManager.initialize(game);
	gameStateManager.activateMainGameState();

	double totalElapsedSeconds = 0;
	double lastSeconds = 0;
	double seconds = glfwGetTime();

	const double timeStep = 1.0 / 60.0;
	double accumulated = 0;

	while (!game.quit && !glfwWindowShouldClose(window)) {
		glfwPollEvents();

		lastSeconds = seconds;
		seconds = glfwGetTime();

		GameState* gameState = gameStateManager.getActiveGameState();

		if (game.paused) {
			render(game, world, camera, *gameState, totalElapsedSeconds);
			continue;
		}

		double elapsedSeconds = (seconds - lastSeconds) * game.speed;

		accumulated += elapsedSeconds;
		totalElapsedSeconds += elapsedSeconds;

		bool updated = false;
		while (accumulated >= timeStep) {
			updated = true;
			accumulated -= timeStep;

			//  Update
			addTransitionTime(timeStep);

			gameState->update(game, timeStep);
			world.update(timeStep);
		}

		if (updated) {
			render(game, world, camera, *gameState, totalElapsedSeconds);
		}

		screenCapture();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
}