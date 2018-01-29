#include "asset_manager.hpp"
#include "camera.hpp"
#include "font.hpp"
#include "gl_headers.hpp"
#include "intro_game_state.hpp"
#include "light.hpp"
#include "map_render.hpp"
#include "render.hpp"
#include "screen_capture.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>

namespace ScaleMail {
bool paused = false;

const int CAPTURE_SKIP_FRAMES = 2;
int captureSkipFrames = 0;
ScreenCapture capture;

//  ============================================================================
static void errorCallback(int error, const char* description) {
    std::cerr << "GL Error " << error << ": " << description << std::endl;
}

//  ============================================================================
static void keyCallback(GLFWwindow* window, int key,
                        [[maybe_unused]] int scancode, int action,
                        [[maybe_unused]] int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        capture.startCapture();
        std::cout << "Capturing started." << std::endl;
    }

    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        capture.stopCapture();
        std::cout << "Capturing stopped." << std::endl;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        paused = !paused;
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

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Glad failed to initialize." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(0);
    glEnable(GL_FRAMEBUFFER_SRGB);

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

    Camera camera(cameraZoom);

    IntroGameState introGameState;
    introGameState.initialize(world, camera);

    double totalElapsedSeconds = 0;
    double lastSeconds = 0;
    double seconds = glfwGetTime();

    const double timeStep = 1.0 / 60.0;
    double accumulated = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        lastSeconds = seconds;
        seconds = glfwGetTime();

        if (paused) {
            continue;
        }

        accumulated += seconds - lastSeconds;

        bool updated = false;
        while (accumulated >= timeStep) {
            updated = true;
            accumulated -= timeStep;

            //  Update
            addTransitionTime(timeStep);
            simulateLights(timeStep);

            introGameState.update(world, camera, timeStep);
            world.update(timeStep);
        }

        if (updated) {
            render(window, world, camera, introGameState, totalElapsedSeconds);
        }

        //  Screen capture
        if (captureSkipFrames > 0) {
            --captureSkipFrames;
        } else {
            capture.captureFrame();
            captureSkipFrames = CAPTURE_SKIP_FRAMES;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
}