#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "light_system.hpp"
#include "math_util.hpp"
#include "mesh.hpp"
#include "quad_shader.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ScaleMail
{
static const GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

static int fboSize = 1024;
static GLuint fboA;
static GLuint fboATexture;
static GLuint fboB;
static GLuint fboBTexture;

static float ticks = 0;

static std::vector<Light> lights;

static Texture lightTexture;

static SpriteBatch lightSpriteBatch;
static SpriteBatch glowSpriteBatch;

static Mesh quadMesh;
static QuadShader quadShader;

static std::vector<unsigned int> textureId;
static std::vector<float> positionX;
static std::vector<float> positionY;
static std::vector<float> colorR;
static std::vector<float> colorG;
static std::vector<float> colorB;
static std::vector<float> colorA;
static std::vector<float> sizeX;
static std::vector<float> sizeY;
static std::vector<float> rotate;
static std::vector<float> texU1;
static std::vector<float> texV1;
static std::vector<float> texU2;
static std::vector<float> texV2;
static std::vector<bool> alpha;

//  ============================================================================
void addLight(glm::vec2 position, glm::vec4 color, float size, float pulse,
              float pulseSize) {
    lights.push_back({
        position,
        color,
        lightTexture.id,
        size,
        pulse,
        pulseSize,
    });
}

//  ============================================================================
static void createFramebuffer(GLuint& fbo, int size, GLuint& fboTexture) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexture, 0);
    glDrawBuffers(1, drawBuffers);
}

//  ============================================================================
static void destroyFramebuffer(GLuint& fbo, GLuint& fboTexture) {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &fboTexture);
}

//  ============================================================================
void initializeLight(AssetManager& assetManager) {
    quadShader = assetManager.getQuadShader();

    quadMesh = assetManager.getQuadMesh();

    lightSpriteBatch.initialize(assetManager);
    glowSpriteBatch.initialize(assetManager);

    lightTexture = assetManager.loadTexture("light");

    createFramebuffer(fboA, fboSize, fboATexture);
    createFramebuffer(fboB, fboSize, fboBTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//  ============================================================================
void buildLightMeshVertexData(SpriteBatch& spriteBatch, float scale) {
    const float u1 = 0;
    const float v1 = 0;
    const float u2 = 1;
    const float v2 = 1;

    textureId.clear();
    positionX.clear();
    positionY.clear();
    colorR.clear();
    colorG.clear();
    colorB.clear();
    colorA.clear();
    sizeX.clear();
    sizeY.clear();
    rotate.clear();
    texU1.clear();
    texV1.clear();
    texU2.clear();
    texV2.clear();
    alpha.clear();

    std::unordered_map<bool, std::unordered_map<unsigned int, int>> textureIdCounts;

    for (const auto& light : lights) {
        ++textureIdCounts[false][light.textureId];

        textureId.push_back(light.textureId);

        float lightSize = (light.pulseSize * sin(ticks * light.pulse)) + light.size;

        float size = lightSize * scale;

        //  Round position to pixel coordinates to prevent wobbling
        glm::vec2 position = glm::vec2(
            (int)light.position.x,
            (int)light.position.y);

        positionX.push_back(position.x);
        positionY.push_back(position.y);
        colorR.push_back(light.color.r);
        colorG.push_back(light.color.g);
        colorB.push_back(light.color.b);
        colorA.push_back(light.color.a);
        sizeX.push_back(size);
        sizeY.push_back(size);
        texU1.push_back(u1);
        texV1.push_back(v2);
        texU2.push_back(u2);
        texV2.push_back(v1);
        alpha.push_back(false);
        rotate.push_back(0);
    }

    spriteBatch.buildSpriteVertexData(
        lights.size(),
        textureIdCounts,
        textureId,
        alpha,
        positionX,
        positionY,
        colorR,
        colorG,
        colorB,
        colorA,
        sizeX,
        sizeY,
        rotate,
        texU1,
        texV1,
        texU2,
        texV2);
}

//  ============================================================================
int getMinFramebufferSize(GLFWwindow* window) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    int max = std::max(width, height);

    return isPowerOfTwo(max) ? max : nextPowerOfTwo(max);
}

//  ============================================================================
void renderLight(GameWindow& gameWindow, Camera& camera,
                 LightSystem& lightSystem) {
    const glm::vec4 ambientColor = lightSystem.getAmbientColor();

    const int minFrameBufferSize = getMinFramebufferSize(gameWindow.window);
    if (fboSize != minFrameBufferSize) {
        fboSize = minFrameBufferSize;
        destroyFramebuffer(fboA, fboATexture);
        destroyFramebuffer(fboB, fboBTexture);
        createFramebuffer(fboA, fboSize, fboATexture);
        createFramebuffer(fboB, fboSize, fboBTexture);
        std::cout << "Light framebuffers resized to " << fboSize << std::endl;
    }

    glm::mat4 quadWorld =
        glm::translate(glm::mat4(1.0f), glm::vec3(fboSize * 0.5f, fboSize * 0.5f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(fboSize * 0.5f, fboSize * 0.5f, 1.0f));

    glm::mat4 fboProjection = glm::ortho(0.0f, (float)fboSize, 0.0f, (float)fboSize, 0.0f, 1.0f);

    glm::mat4 screenProjection = glm::ortho(
        0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
        0.0f, 1.0f);

    glm::mat4 lightMvp = fboProjection * camera.getView();

    glm::mat4 screenMvp = screenProjection * quadWorld;

    glEnable(GL_BLEND);

    //  Draw lights to FBO A
    glowSpriteBatch.begin();
    buildLightMeshVertexData(glowSpriteBatch, 0.25f);
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, fboA);
    glViewport(0, 0, fboSize, fboSize);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, lightTexture.id);
    glowSpriteBatch.render(lightMvp);
    glowSpriteBatch.end();

    //  Draw lights to FBO B
    lightSpriteBatch.begin();
    buildLightMeshVertexData(lightSpriteBatch, 1.0f);
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, fboB);
    glViewport(0, 0, fboSize, fboSize);
    glClearColor(ambientColor.r, ambientColor.g, ambientColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightSpriteBatch.render(lightMvp);
    lightSpriteBatch.end();

    //  Draw lights from FBO A to screen
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gameWindow.width, gameWindow.height);
    glUseProgram(quadShader.id);
    glUniformMatrix4fv(quadShader.mvpLocation, 1, GL_FALSE, &screenMvp[0][0]);
    glBindTexture(GL_TEXTURE_2D, fboATexture);
    glBindVertexArray(quadMesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, quadMesh.vertexCount);

    //  Draw lights from FBO B to screen
    blendModulate();
    glBindTexture(GL_TEXTURE_2D, fboBTexture);
    glDrawArrays(GL_TRIANGLES, 0, quadMesh.vertexCount);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

//  ============================================================================
void simulateLights(float elapsedSeconds) {
    ticks += elapsedSeconds;
}
}