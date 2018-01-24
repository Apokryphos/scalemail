#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "math_util.hpp"
#include "mesh.hpp"
#include "shader.hpp"
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

static size_t lightMeshVertexBufferSize = 0;
static Mesh lightMesh;
static std::vector<float> lightMeshVertexData;

static GLuint spriteShader;
static GLuint spriteShaderMvpLocation;

static Mesh quadMesh;
static GLuint quadShader;
static GLuint quadShaderMvpLocation;

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
static bool initLightMesh(Mesh& mesh) {
    glGenVertexArrays(1, &lightMesh.vao);
    glGenBuffers(1, &lightMesh.vbo);

    mesh.vertexCount = 0;

    glBindVertexArray(lightMesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, lightMesh.vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) (sizeof(float) * 2));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) (sizeof(float) * 6));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
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
void initializeLight() {
    initShaderProgram("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", spriteShader);
    spriteShaderMvpLocation = glGetUniformLocation(spriteShader, "MVP");

    initQuadMesh(quadMesh);
    initShaderProgram("assets/shaders/flat.vert", "assets/shaders/flat.frag", quadShader);
    quadShaderMvpLocation = glGetUniformLocation(quadShader, "MVP");

    initLightMesh(lightMesh);
    loadPngTexture("assets/textures/light.png", lightTexture);

    createFramebuffer(fboA, fboSize, fboATexture);
    createFramebuffer(fboB, fboSize, fboBTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//  ============================================================================
void buildLightMeshVertexData(float scale) {
    int lightCount = 0;

    float u1 = 0;
    float v1 = 0;

    float u2 = 1;
    float v2 = 1;

    for (const auto& light : lights) {
        float lightSize = (light.pulseSize * sin(ticks * light.pulse)) + light.size;

        glm::vec2 size = glm::vec2(lightSize) * scale;

        glm::vec2 position = light.position - size * 0.5f;

        ++lightCount;

        lightMeshVertexData.push_back(position.x);
        lightMeshVertexData.push_back(position.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u1);
        lightMeshVertexData.push_back(v1);

        lightMeshVertexData.push_back(position.x);
        lightMeshVertexData.push_back(position.y + size.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u1);
        lightMeshVertexData.push_back(v2);

        lightMeshVertexData.push_back(position.x + size.x);
        lightMeshVertexData.push_back(position.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u2);
        lightMeshVertexData.push_back(v1);

        lightMeshVertexData.push_back(position.x);
        lightMeshVertexData.push_back(position.y + size.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u1);
        lightMeshVertexData.push_back(v2);

        lightMeshVertexData.push_back(position.x + size.x);
        lightMeshVertexData.push_back(position.y + size.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u2);
        lightMeshVertexData.push_back(v2);

        lightMeshVertexData.push_back(position.x + size.x);
        lightMeshVertexData.push_back(position.y);
        lightMeshVertexData.push_back(light.color.r);
        lightMeshVertexData.push_back(light.color.g);
        lightMeshVertexData.push_back(light.color.b);
        lightMeshVertexData.push_back(light.color.a);
        lightMeshVertexData.push_back(u2);
        lightMeshVertexData.push_back(v1);
    }

    lightMesh.vertexCount = lightCount * 6;

    glBindBuffer(GL_ARRAY_BUFFER, lightMesh.vbo);

    size_t vertexDataSize = sizeof(float) * lightMeshVertexData.size();

    if (lightMeshVertexBufferSize < vertexDataSize) {
        lightMeshVertexBufferSize = vertexDataSize;

        glBufferData(GL_ARRAY_BUFFER, lightMeshVertexBufferSize,
                     &lightMeshVertexData[0], GL_STATIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, lightMeshVertexBufferSize,
                        &lightMeshVertexData[0]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    lightMeshVertexData.clear();
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
void renderLight(GameWindow& gameWindow, Camera& camera, glm::vec4 ambientColor) {
    buildLightMeshVertexData(0.25f);

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
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, fboA);
    glViewport(0, 0, fboSize, fboSize);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(spriteShader);
    glUniformMatrix4fv(spriteShaderMvpLocation, 1, GL_FALSE, &lightMvp[0][0]);
    glBindTexture(GL_TEXTURE_2D, lightTexture.id);
    glBindVertexArray(lightMesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, lightMesh.vertexCount);

    buildLightMeshVertexData(1.0f);

    //  Draw lights to FBO B
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, fboB);
    glViewport(0, 0, fboSize, fboSize);
    glClearColor(ambientColor.r, ambientColor.g, ambientColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, lightTexture.id);
    glBindVertexArray(lightMesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, lightMesh.vertexCount);

    //  Draw lights from FBO A to screen
    blendAdditive();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gameWindow.width, gameWindow.height);
    glUseProgram(quadShader);
    glUniformMatrix4fv(quadShaderMvpLocation, 1, GL_FALSE, &screenMvp[0][0]);
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