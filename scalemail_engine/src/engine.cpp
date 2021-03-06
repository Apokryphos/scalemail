#include "gui/gui.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "camera_system.hpp"
#include "cursor.hpp"
#include "engine_start_options.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "gl_headers.hpp"
#include "gui/imgui.hpp"
#include "gui/imgui_glfw.hpp"
#include "input.hpp"
#include "input_device.hpp"
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
float calculateCameraZoom(GameWindow& gameWindow) {
	int screenSize;
	if (gameWindow.getFullscreen()) {
		screenSize = std::min(gameWindow.getWidth(), gameWindow.getHeight());
	} else {
		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;
		glfwGetWindowFrameSize(gameWindow.getGlfwWindow(), &left, &top, &right,
							   &bottom);

		const int sh = gameWindow.getWidth() - left - right;
		const int sv = gameWindow.getHeight() - top - bottom;
		screenSize = std::min(sh, sv);
	}

	const float cz = screenSize / 256.0f;
	const float cameraZoom = std::floor(cz);

	return cameraZoom;
}

//  ============================================================================
static void errorCallback(int error, const char* description) {
	std::cerr << "GL Error " << error << ": " << description << std::endl;
}

//  ============================================================================
static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	GameWindow& gameWindow = game->gameWindow;

	gameWindow.resize();

	World* world = game->world;

	if (world != nullptr) {
		CameraSystem& cameraSystem = world->getCameraSystem();

		cameraSystem.resizeCameras(
			gameWindow.getWidth(),
			gameWindow.getHeight(),
			calculateCameraZoom(gameWindow));
	}

	std::cout << "Framebuffer resized to " << width << "x" << height << std::endl;
}

//  ============================================================================
static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
						int mods) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	DevGui& devGui = game->devGui;

	bool inputConsumed = false;

	if (devGui.getVisible()) {
		imGuiKeyCallback(window, key, scancode, action, mods);
		inputConsumed = devGui.getKeyboardInputConsumed();
	}

	if (!inputConsumed) {
		inputKeyCallback(window, key, scancode, action, mods);
	}
}

//  ============================================================================
static void mouseButtonCallback(GLFWwindow* window, int button, int action,
								int mods) {
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	DevGui& devGui = game->devGui;

	bool inputConsumed = false;

	if (devGui.getVisible()) {
		imGuiMouseButtonCallback(window, button, action, mods);
		inputConsumed = devGui.getMouseInputConsumed();
	}

	if (!inputConsumed) {
		inputMouseButtonCallback(window, button, action, mods);
	}
}

//  ============================================================================
static void initializeInputCallbacks(GLFWwindow* window) {
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
}

//  ============================================================================
static void screenCapture() {
	InputState& inputState = getKeyboardInputDevice().getInputState();

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
GLFWwindow* initializeGlfw(const EngineStartOptions& startOptions) {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) {
		std::cerr << "GLFW failed to initialize." << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	//	Hide window
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	int screenWidth =
		startOptions.screenWidth > 0 ?
		startOptions.screenWidth :
		mode->width;

	int screenHeight =
		startOptions.screenHeight > 0 ?
		startOptions.screenHeight :
		mode->height;

	//	Hints to use current video mode settings
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	GLFWwindow* window = glfwCreateWindow(
		screenWidth, screenHeight, "ScaleMail",
		(startOptions.fullscreen ? monitor : NULL), NULL);

	if (!window) {
		std::cerr << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// glfwSetWindowSizeLimits(window, screenWidth, screenHeight,
	// 						screenWidth, screenHeight);


	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		std::cerr << "Glad failed to initialize." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	glfwSwapInterval(1);
	glEnable(GL_FRAMEBUFFER_SRGB);

	return window;
}

//  ============================================================================
void initializeRenderConfig(GLFWwindow* window,
							const EngineStartOptions& startOptions,
							RenderCaps& renderCaps,
							RenderOptions& renderOptions) {
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
}

//  ============================================================================
int startEngine(EngineStartOptions startOptions) {
	GLFWwindow* glfwWindow = initializeGlfw(startOptions);

	if (glfwWindow == nullptr) {
		return -1;
	}

	RenderCaps renderCaps = {};
	RenderOptions renderOptions = {};
	initializeRenderConfig(glfwWindow, startOptions, renderCaps, renderOptions);

	AssetManager assetManager;
	assetManager.initialize(renderCaps);

	capture.initialize(glfwWindow);
	initializeInput(glfwWindow);
	loadCursor(glfwWindow);

	initializeRender(assetManager);

	initializeImgui(glfwWindow, assetManager);

	World world;
	world.initialize(assetManager);

	std::string mapName =
		startOptions.mapName.empty() ? "map1" : startOptions.mapName;

	//  Load map after all other initialize functions
	world.loadMap(mapName);

	buildAmbientLights(world.getMap()->getAmbientLights());

	Gui gui;
	gui.initialize(assetManager);

	Game game = {};
	game.devOptions.enabled = true;
	game.devOptions.stepCount = 1;
	game.renderCaps = renderCaps;
	game.renderOptions = renderOptions;
	game.gui = &gui;
	game.speed = 1.0;
	game.world = &world;
	game.gameWindow.setGlfwWindow(glfwWindow);
	glfwSetWindowUserPointer(glfwWindow, &game);

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.setCameraDefaults(
		game.gameWindow.getWidth(),
		game.gameWindow.getHeight(),
		calculateCameraZoom(game.gameWindow));

	GameStateManager gameStateManager;
	gameStateManager.initialize(game);

	if (mapName == "map1") {
		gameStateManager.activateIntroGameState(startOptions.skipIntro);
	} else {
		gameStateManager.activateMainGameState();
	}

	initializeInputCallbacks(glfwWindow);

	//	Center and show window
	game.gameWindow.center();
	glfwShowWindow(glfwWindow);

	double totalElapsedSeconds = 0;
	double lastSeconds = 0;
	double seconds = glfwGetTime();

	double accumulated = 0;

	while (!game.quit && !glfwWindowShouldClose(glfwWindow)) {
		glfwPollEvents();

		lastSeconds = seconds;
		seconds = glfwGetTime();

		if (game.camera != nullptr) {
			game.camera->setDevMode(game.devOptions.camera3d);
		}

		GameState* gameState = gameStateManager.getActiveGameState();

		//	If paused, render frame and continue loop
		if (game.paused && !game.devOptions.step) {
			render(game, world, *gameState, totalElapsedSeconds);
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
			render(game, world, *gameState, totalElapsedSeconds);
			screenCapture();
		}
	}

	shutdownImGui();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();

	return 0;
}
}