#include "blend.hpp"
#include "camera.hpp"
#include "ease.hpp"
#include "fake_map.hpp"
#include "font.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "render.hpp"
#include "screen_capture.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "transition.hpp"
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

    capture.initialize(window);

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Glad failed to initialize." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);
    glEnable(GL_FRAMEBUFFER_SRGB);

    initializeFont();

    initializeTransition();
    setTransitionState(TransitionState::FADED_OUT);

    initializeLight();
    initializeSprites();

    //  Do this last after all other initialize functions
    initializeFakeMap();

    glm::vec4 startAmbientColor(0.14f, 0.064f, 0.04f, 1.0f);
    // glm::vec4 endAmbientColor(0.2f, 0.28f, 0.3f, 1.0f);
    glm::vec4 endAmbientColor(0.3f, 0.38f, 0.4f, 1.0f);
    glm::vec4 ambientColor = startAmbientColor;

    const float STATE1_DURATION = 0.5f;
    const float STATE2_DURATION = 2.0f;
    const float STATE3_DURATION = 3.0f;
    const float STATE4_DURATION = 9.0f;
    const float STATE5_DURATION = 1.0f;

    int introState = 0;
    float introTicks = 0;
    float textAlpha = 0.0f;

    const float introCameraStartY = 0.0f;
    const float introCameraEndY = -768.0f;

    Camera camera(cameraZoom);
    camera.position = glm::vec2(0, introCameraStartY);

    Texture mapTexture;
    loadPngTexture("assets/textures/map.png", mapTexture);

    Mesh mapMesh;
    initQuadMesh(mapMesh);

    GLuint shader;
    GLuint shaderMvpLocation;
    initShaderProgram("assets/shaders/flat.vert", "assets/shaders/flat.frag", shader);
    shaderMvpLocation = glGetUniformLocation(shader, "MVP");

    glm::mat4 world =
        glm::translate(glm::mat4(1.0f), glm::vec3(128.0f, 512.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(128.0f, 512.0f, 1.0f));

    float elapsedSeconds = 0;
    double lastSeconds = 0;
    double seconds = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        lastSeconds = seconds;
        seconds = glfwGetTime();
        elapsedSeconds = (float)(seconds - lastSeconds);

        if (paused) {
            elapsedSeconds = 0;
        }

        addTransitionTime(elapsedSeconds);
        simulateLights(elapsedSeconds);
        simulateSprites(elapsedSeconds);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f,
                                          0.0f, 1.0f);

        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        switch (introState) {
            //  Pause before fading title in
            case 0:
                introTicks += elapsedSeconds;
                if (introTicks >= STATE1_DURATION) {
                    introTicks = 0;
                    ++introState;
                }
                break;

            //  Fade title in
            case 1:
                introTicks += elapsedSeconds;
                textAlpha = easeInCubic(introTicks, 0, 1, STATE2_DURATION);
                if (introTicks >= STATE2_DURATION) {
                    introTicks = 0;
                    transitionFadeIn();
                    ++introState;
                }
                break;

            //  Pause after fading in map
            case 2:
                introTicks += elapsedSeconds;
                textAlpha = 1.0f;
                if (introTicks >= STATE3_DURATION) {
                    introTicks = 0;
                    ++introState;
                }
                break;

            //  Scroll down and fade out title
            case 3:
                introTicks += elapsedSeconds;
                textAlpha = 1 - easeOutCubic(introTicks, 0, 1, STATE2_DURATION);

                ambientColor.r = easeInOutCubic(
                    introTicks,
                    startAmbientColor.r,
                    endAmbientColor.r - startAmbientColor.r,
                    STATE4_DURATION);

                ambientColor.g = easeInOutCubic(
                    introTicks,
                    startAmbientColor.g,
                    endAmbientColor.g - startAmbientColor.g,
                    STATE4_DURATION);

                ambientColor.b = easeInOutCubic(
                    introTicks,
                    startAmbientColor.b,
                    endAmbientColor.b - startAmbientColor.b,
                    STATE4_DURATION);

                ambientColor.a = easeInOutCubic(
                    introTicks,
                    startAmbientColor.a,
                    endAmbientColor.a - startAmbientColor.a,
                    STATE4_DURATION);

                camera.position.y = easeInOutCubic(
                    introTicks,
                    introCameraStartY,
                    introCameraEndY - introCameraStartY,
                    STATE4_DURATION);

                if (introTicks >= STATE4_DURATION) {
                    introTicks = 0;
                    camera.position.y = introCameraEndY;
                    ++introState;
                }
                break;

            //  Pause before fading out
            case 4:
                introTicks += elapsedSeconds;
                if (introTicks >= STATE5_DURATION) {
                    introTicks = 0;
                    transitionFadeOut();
                    ++introState;
                }
        }

        glm::mat4 mvp = projection * camera.getView() * world;

        drawCenterText(
            glm::vec2(
                width * 0.5f,
                height * 0.5f - 32.0f),
            "- SCALEMAIL -",
            glm::vec4(1.0f, 1.0f, 1.0f, textAlpha),
            8.0f * cameraZoom);

        blendNone();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(shader);
        glUniformMatrix4fv(shaderMvpLocation, 1, GL_FALSE, &mvp[0][0]);
        glBindTexture(GL_TEXTURE_2D, mapTexture.id);
        glBindVertexArray(mapMesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, mapMesh.vertexCount);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        render(window, camera, ambientColor);

        glfwSwapBuffers(window);

        if (captureSkipFrames > 0) {
            --captureSkipFrames;
        } else {
            capture.captureFrame();
            captureSkipFrames = CAPTURE_SKIP_FRAMES;
        }

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
}