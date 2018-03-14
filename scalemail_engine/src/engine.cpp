#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "cursor.hpp"
#include "engine_start_options.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "gl_headers.hpp"
#include "input.hpp"
#include "input_state.hpp"
#include "map.hpp"
#include "map_render.hpp"
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
				   double lastSeconds,
				   double& accumulated) {
	bool updated = false;
	while (accumulated >= TIME_STEP) {
		updated = true;
		accumulated -= TIME_STEP;
		lastSeconds += TIME_STEP;

		addTransitionTime(TIME_STEP);
		gameState->update(game, TIME_STEP);
		world.update(lastSeconds, TIME_STEP);

		game.gui->update(TIME_STEP);

		updateMapMeshAnimation(TIME_STEP);
	}

	return updated;
}

//  ============================================================================
int startEngine(EngineStartOptions startOptions) {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	const int screenWidth =
		startOptions.screenWidth > 0 ?
		startOptions.screenWidth :
		mode->width;

	const int screenHeight =
		startOptions.screenHeight > 0 ?
		startOptions.screenHeight :
		mode->height;

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight,
										  "ScaleMail", NULL, NULL);

	if (!window) {
		std::cerr << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeLimits(window, screenWidth, screenHeight,
							screenWidth, screenHeight);

	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

	const int sh = screenWidth - left - right;
	const int sv = screenHeight - top - bottom;
	const int screenSize = std::min(sh, sv);

	const float cz = screenSize / 256.0f;
	const float cameraZoom = static_cast<int>(cz * 2) / 2.0f;

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

	RenderCaps renderCaps = {};
	RenderOptions renderOptions = {};

	const int majorVersion =
		glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);

	const int minorVersion =
		glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

	std::cout << "OpenGL v" << majorVersion << "." << minorVersion << std::endl;

	if (startOptions.openGl2) {
		std::cout << "Forcing OpenGL 2.1 usage." << std::endl;
	}

	if (!startOptions.openGl2 && majorVersion >= 3 && minorVersion >= 3) {
		//	OpenGL 3.3 - GLSL 330
		renderCaps.shaderVersion = ShaderVersion::SHADER_VERSION_330;
		renderCaps.fboSupported = true;

		renderOptions.lightsEnabled = true;

		std::cout << "Using GLSL 330 shaders." << std::endl;
	} else {
		//	OpenGL 2.1 - GLSL 120
		renderCaps.shaderVersion = ShaderVersion::SHADER_VERSION_120;

		std::cout << "Using GLSL 120 shaders." << std::endl;
	}

	loadCursor(window);

	AssetManager assetManager;
	assetManager.initialize(renderCaps);

	initializeRender(assetManager);

	//  Load map after all other initialize functions
	World world;
	world.initialize(assetManager);

	std::string mapName =
		startOptions.mapName.empty() ? "map1" : startOptions.mapName;

	world.loadMap(mapName);

	buildAmbientLights(world.getMap()->getAmbientLights());

	Camera camera(screenWidth, screenHeight, cameraZoom);

	Gui gui;

	Game game = {};
	game.devOptions.enabled = true;
	game.devOptions.stepCount = 1;
	game.renderCaps = renderCaps;
	game.renderOptions = renderOptions;
	game.camera = &camera;
	game.gameWindow.window = window;
	game.gui = &gui;
	game.speed = 1.0;
	game.world = &world;
	glfwSetWindowUserPointer(window, &game);

	gui.initialize(assetManager);

	GameStateManager gameStateManager;
	gameStateManager.initialize(game);

	if (startOptions.skipIntro) {
		gameStateManager.activateMainGameState();
	} else {
		gameStateManager.activateIntroGameState();
	}

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

		double elapsedSeconds = (seconds - lastSeconds) * game.speed;

		bool updated = false;
		if (game.devOptions.step) {
			//	Step mode update
			game.paused = false;

			double step = game.devOptions.stepCount * TIME_STEP;
			update(game, world, gameState, lastSeconds, step);

			game.paused = true;
			game.devOptions.step = false;
			continue;
		} else {
			//	Normal update
			accumulated += elapsedSeconds;
			totalElapsedSeconds += elapsedSeconds;

			updated = update(game, world, gameState, lastSeconds, accumulated);
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