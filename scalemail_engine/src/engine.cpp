#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "cursor.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "gl_headers.hpp"
#include "input.hpp"
#include "input_state.hpp"
#include "render.hpp"
#include "screen_capture.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <iostream>

namespace ScaleMail {

const double TIME_STEP = 1.0 / 60.0;

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
static bool update(Game& game, World& world, GameState* gameState,
				   double& accumulated) {
	bool updated = false;
	while (accumulated >= TIME_STEP) {
		updated = true;
		accumulated -= TIME_STEP;

		addTransitionTime(TIME_STEP);
		gameState->update(game, TIME_STEP);
		world.update(TIME_STEP);
	}

	return updated;
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

	initializeRender(assetManager);

	//  Load map after all other initialize functions
	World world;
	world.initialize(&assetManager);

	// world.loadMap("map1");
	world.loadMap("test_map");

	buildAmbientLights();

	Camera camera(screenWidth, screenHeight, cameraZoom);

	Gui gui;

	Game game = {};
	game.devOptions.enabled = true;
	game.devOptions.stepCount = 1;
	game.camera = &camera;
	game.gameWindow.window = window;
	game.gui = &gui;
	game.speed = 1.0;
	game.world = &world;
	glfwSetWindowUserPointer(window, &game);

	gui.initialize(assetManager);

	GameStateManager gameStateManager;
	gameStateManager.initialize(game);
	gameStateManager.activateMainGameState();

	double totalElapsedSeconds = 0;
	double lastSeconds = 0;
	double seconds = glfwGetTime();

	double accumulated = 0;

	while (!game.quit && !glfwWindowShouldClose(window)) {
		glfwPollEvents();

		lastSeconds = seconds;
		seconds = glfwGetTime();

		camera.setDevMode(game.devOptions.camera3d);

		GameState* gameState = gameStateManager.getActiveGameState();

		//	If paused, render frame and continue loop
		if (game.paused && !game.devOptions.step) {
			render(game, world, camera, *gameState, totalElapsedSeconds);
			continue;
		}

		bool updated = false;
		if (game.devOptions.step) {
			//	Step mode update
			game.paused = false;

			double step = game.devOptions.stepCount * TIME_STEP;
			update(game, world, gameState, step);

			game.paused = true;
			game.devOptions.step = false;
			continue;
		} else {
			//	Normal update
			double elapsedSeconds = (seconds - lastSeconds) * game.speed;

			accumulated += elapsedSeconds;
			totalElapsedSeconds += elapsedSeconds;

			updated = update(game, world, gameState, accumulated);
		}

		//	Only render after an update
		if (updated) {
			render(game, world, camera, *gameState, totalElapsedSeconds);
			screenCapture();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
}