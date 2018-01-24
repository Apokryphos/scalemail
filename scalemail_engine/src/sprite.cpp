#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "sprite.hpp"
#include "texture.hpp"
#include "tileset.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace ScaleMail
{
static std::vector<Sprite> actorSprites;
static std::vector<Sprite> worldSprites;

static size_t spriteMeshVertexBufferSize = 0;
static Mesh spriteMesh;
static std::vector<float> spriteMeshVertexData;

static GLuint spriteShader;
static GLuint spriteShaderMvpLocation;

static Texture actorsTexture;
static Texture worldTexture;

//  ============================================================================
void getActorSpriteAnimation(const int actorIndex, SpriteAnimation& anim) {
    const int SPRITES_PER_ACTOR = 8;
    const int SPRITES_PER_ROW = 16;

    int spriteX = actorIndex % 4;
    int spriteY = actorIndex / 4 * 2;

    int spriteIndex = spriteX * 4 + spriteY * SPRITES_PER_ROW;
    // int spriteIndex = actorIndex * SPRITES_PER_ACTOR;

    const int east = static_cast<int>(Direction::EAST);
    const int south = static_cast<int>(Direction::SOUTH);
    const int north = static_cast<int>(Direction::NORTH);
    const int west = static_cast<int>(Direction::WEST);

    anim.frames[0].tilesetIds[east]  = spriteIndex;
    anim.frames[0].tilesetIds[south] = spriteIndex + 1;
    anim.frames[0].tilesetIds[north] = spriteIndex + 2;
    anim.frames[0].tilesetIds[west]  = spriteIndex + 3;

    anim.frames[1].tilesetIds[east]  = spriteIndex + SPRITES_PER_ROW;
    anim.frames[1].tilesetIds[south] = spriteIndex + 1 + SPRITES_PER_ROW;
    anim.frames[1].tilesetIds[north] = spriteIndex + 2 + SPRITES_PER_ROW;
    anim.frames[1].tilesetIds[west]  = spriteIndex + 3 + SPRITES_PER_ROW;
}

//  ============================================================================
void getWorldSpriteAnimation(const int index, SpriteAnimation& anim) {
    const int east = static_cast<int>(Direction::EAST);
    const int south = static_cast<int>(Direction::SOUTH);
    const int north = static_cast<int>(Direction::NORTH);
    const int west = static_cast<int>(Direction::WEST);

    switch (index) {
        //  Torch
        case 197:
        //  Torch cast sprite
        case 199:
            anim.frames[0].tilesetIds[east]  = index;
            anim.frames[0].tilesetIds[south] = index;
            anim.frames[0].tilesetIds[north] = index;
            anim.frames[0].tilesetIds[west]  = index;
            anim.frames[1].tilesetIds[east]  = index + 1;
            anim.frames[1].tilesetIds[south] = index + 1;
            anim.frames[1].tilesetIds[north] = index + 1;
            anim.frames[1].tilesetIds[west]  = index + 1;
            break;

        //  Torch
        case 198:
        //  Torch cast sprite
        case 200:
            anim.frames[0].tilesetIds[east]  = index;
            anim.frames[0].tilesetIds[south] = index;
            anim.frames[0].tilesetIds[north] = index;
            anim.frames[0].tilesetIds[west]  = index;
            anim.frames[1].tilesetIds[east]  = index - 1;
            anim.frames[1].tilesetIds[south] = index - 1;
            anim.frames[1].tilesetIds[north] = index - 1;
            anim.frames[1].tilesetIds[west]  = index - 1;
            break;

        default:
            anim.frames[0].tilesetIds[south] = index;
            anim.frames[0].tilesetIds[north] = index;
            anim.frames[0].tilesetIds[west]  = index;
            anim.frames[1].tilesetIds[east]  = index;
            anim.frames[1].tilesetIds[south] = index;
            anim.frames[1].tilesetIds[north] = index;
            anim.frames[1].tilesetIds[west]  = index;
            std::cerr << "getWorldSpriteAnimation case not implemented: "
                      << index << std::endl;
                         anim.frames[0].tilesetIds[east]  = index;
            break;
    }
}

//  ============================================================================
void addActorSprite(glm::vec2 position, int actorIndex, Direction facing) {
    Sprite sprite;
    sprite.facing = facing;
    sprite.textureId = actorsTexture.id;
    sprite.size = 16.0f;
    sprite.position = position;
    sprite.color = glm::vec4(1.0f);

    sprite.animation.frameIndex = 0;
    sprite.animation.ticks = 0;
    sprite.animation.frames[0].duration  = 0.3f;
    sprite.animation.frames[1].duration  = 0.3f;

    getActorSpriteAnimation(actorIndex, sprite.animation);

    actorSprites.push_back(sprite);
}

//  ============================================================================
void addWorldSprite(glm::vec2 position, int tilesetId) {
    Sprite sprite;
    sprite.facing = Direction::SOUTH;
    sprite.textureId = worldTexture.id;
    sprite.size = 16.0f;
    sprite.position = position + glm::vec2(8.0f, -8.0f);
    sprite.color = glm::vec4(1.0f);

    sprite.animation.frameIndex = 0;
    sprite.animation.ticks = 0;
    sprite.animation.frames[0].duration  = 0.3f;
    sprite.animation.frames[1].duration  = 0.3f;

    getWorldSpriteAnimation(tilesetId, sprite.animation);

    worldSprites.push_back(sprite);
}

//  ============================================================================
static bool initSpriteMesh(Mesh& mesh) {
    glGenVertexArrays(1, &spriteMesh.vao);
    glGenBuffers(1, &spriteMesh.vbo);

    mesh.vertexCount = 0;

    glBindVertexArray(spriteMesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, spriteMesh.vbo);

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
void buildSpriteVertexData(std::vector<Sprite>& sprites) {
    int spriteCount = 0;

    glm::vec2 uv1, uv2;

    for (const auto& sprite : sprites) {
        glm::vec2 size = glm::vec2(sprite.size);

        glm::vec2 position = sprite.position - size * 0.5f;

        const int frameIndex = sprite.animation.frameIndex;
        const int direction = static_cast<int>(sprite.facing);

        const int tilesetId =
            sprite.animation.frames[frameIndex].tilesetIds[direction];

        if (sprite.textureId == actorsTexture.id) {
            getTilesetUv(tilesetId, 256, 512, 16, 16, uv1, uv2);
        } else {
            getTilesetUv(tilesetId, 256, 304, 16, 16, uv1, uv2);
        }

        float u1 = uv1.x;
        float v1 = uv1.y;

        float u2 = uv2.x;
        float v2 = uv2.y;

        ++spriteCount;

        spriteMeshVertexData.push_back(position.x);
        spriteMeshVertexData.push_back(position.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u1);
        spriteMeshVertexData.push_back(v1);

        spriteMeshVertexData.push_back(position.x);
        spriteMeshVertexData.push_back(position.y + size.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u1);
        spriteMeshVertexData.push_back(v2);

        spriteMeshVertexData.push_back(position.x + size.x);
        spriteMeshVertexData.push_back(position.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u2);
        spriteMeshVertexData.push_back(v1);

        spriteMeshVertexData.push_back(position.x);
        spriteMeshVertexData.push_back(position.y + size.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u1);
        spriteMeshVertexData.push_back(v2);

        spriteMeshVertexData.push_back(position.x + size.x);
        spriteMeshVertexData.push_back(position.y + size.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u2);
        spriteMeshVertexData.push_back(v2);

        spriteMeshVertexData.push_back(position.x + size.x);
        spriteMeshVertexData.push_back(position.y);
        spriteMeshVertexData.push_back(sprite.color.r);
        spriteMeshVertexData.push_back(sprite.color.g);
        spriteMeshVertexData.push_back(sprite.color.b);
        spriteMeshVertexData.push_back(sprite.color.a);
        spriteMeshVertexData.push_back(u2);
        spriteMeshVertexData.push_back(v1);
    }

    spriteMesh.vertexCount = spriteCount * 6;

    glBindBuffer(GL_ARRAY_BUFFER, spriteMesh.vbo);

    size_t vertexDataSize = sizeof(float) * spriteMeshVertexData.size();

    if (spriteMeshVertexBufferSize < vertexDataSize) {
        spriteMeshVertexBufferSize = vertexDataSize;

        glBufferData(GL_ARRAY_BUFFER, spriteMeshVertexBufferSize,
                     &spriteMeshVertexData[0], GL_STATIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, spriteMeshVertexBufferSize,
                        &spriteMeshVertexData[0]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    spriteMeshVertexData.clear();
}

//  ============================================================================
void initializeSprites() {
    initShaderProgram("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", spriteShader);
    spriteShaderMvpLocation = glGetUniformLocation(spriteShader, "MVP");

    loadPngTexture("assets/textures/actors.png", actorsTexture);
    loadPngTexture("assets/textures/world.png", worldTexture);

    initSpriteMesh(spriteMesh);
}

//  ============================================================================
static void renderSprites(std::vector<Sprite>& sprites, int textureId,
                          GameWindow& gameWindow, Camera& camera) {
    buildSpriteVertexData(sprites);

    glm::mat4 screenProjection =
        glm::ortho(0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
                   0.0f, 1.0f);

    glm::mat4 screenMvp = screenProjection * camera.getView();

    glEnable(GL_BLEND);

    //  Draw sprites
    blendAlpha();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gameWindow.width, gameWindow.height);
    glUseProgram(spriteShader);
    glUniformMatrix4fv(spriteShaderMvpLocation, 1, GL_FALSE, &screenMvp[0][0]);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindVertexArray(spriteMesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, spriteMesh.vertexCount);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

//  ============================================================================
void renderSprites(GameWindow& gameWindow, Camera& camera) {
    renderSprites(worldSprites, worldTexture.id, gameWindow, camera);
    renderSprites(actorSprites, actorsTexture.id, gameWindow, camera);

}

//  ============================================================================
static void simulateSprites(std::vector<Sprite>& sprites, float elapsedSeconds) {
    for (auto& sprite : sprites) {
        sprite.animation.ticks += elapsedSeconds;

        const float duration = sprite.animation.frames[sprite.animation.frameIndex].duration;

        if (sprite.animation.ticks >= duration) {
            sprite.animation.ticks -= duration;
            ++sprite.animation.frameIndex;

            if (sprite.animation.frameIndex > 1) {
                sprite.animation.frameIndex = 0;
            }
        }
    }
}

//  ============================================================================
void simulateSprites(float elapsedSeconds) {
    simulateSprites(actorSprites, elapsedSeconds);
    simulateSprites(worldSprites, elapsedSeconds);
}
}