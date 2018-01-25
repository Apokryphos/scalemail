#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include "tileset.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ScaleMail
{
static std::vector<Sprite> actorSprites;
static std::vector<Sprite> worldSprites;

static SpriteBatch spriteBatch;

static Texture actorsTexture;
static Texture worldTexture;

//  ============================================================================
void getActorSpriteAnimation(const int actorIndex, SpriteAnimation& anim) {
    const int ACTORS_PER_ROW = 4;
    const int SPRITES_PER_ROW = 16;

    int spriteX = actorIndex % ACTORS_PER_ROW;
    int spriteY = actorIndex / ACTORS_PER_ROW * 2;

    int spriteIndex = spriteX * ACTORS_PER_ROW + spriteY * SPRITES_PER_ROW;

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
void buildSpriteVertexData(std::vector<Sprite>& sprites) {
    std::vector<unsigned int> textureId;
    std::vector<float> positionX;
    std::vector<float> positionY;
    std::vector<float> sizeX;
    std::vector<float> sizeY;
    std::vector<float> rotate;
    std::vector<float> texU1;
    std::vector<float> texV1;
    std::vector<float> texU2;
    std::vector<float> texV2;
    std::vector<bool> alpha;

    glm::vec2 uv1, uv2;

    std::unordered_map<bool, std::unordered_map<unsigned int, int>> textureIdCounts;

    for (const auto& sprite : sprites) {
        ++textureIdCounts[true][sprite.textureId];

        const int frameIndex = sprite.animation.frameIndex;
        const int direction = static_cast<int>(sprite.facing);

        const int tilesetId =
            sprite.animation.frames[frameIndex].tilesetIds[direction];

        if (sprite.textureId == actorsTexture.id) {
            getTilesetUv(tilesetId, 256, 512, 16, 16, uv1, uv2);
        } else {
            getTilesetUv(tilesetId, 256, 304, 16, 16, uv1, uv2);
        }

        textureId.push_back(sprite.textureId);

        //  Round position to pixel coordinates to prevent wobbling
        glm::vec2 position = glm::vec2(
            (int)sprite.position.x,
            (int)sprite.position.y);

        positionX.push_back(position.x);
        positionY.push_back(position.y);
        sizeX.push_back(sprite.size);
        sizeY.push_back(sprite.size);
        texU1.push_back(uv1.x);
        texV1.push_back(uv2.y);
        texU2.push_back(uv2.x);
        texV2.push_back(uv1.y);
        alpha.push_back(true);
        rotate.push_back(0);
    }

    spriteBatch.buildSpriteVertexData(
        sprites.size(),
        textureIdCounts,
        textureId,
        alpha,
        positionX,
        positionY,
        sizeX,
        sizeY,
        rotate,
        texU1,
        texV1,
        texU2,
        texV2);
}

//  ============================================================================
void initializeSprites(AssetManager& assetManager) {
    spriteBatch.initialize(assetManager);

    actorsTexture = assetManager.loadTexture("actors");
    worldTexture = assetManager.loadTexture("world");
}

//  ============================================================================
void renderSprites(GameWindow& gameWindow, Camera& camera) {
    glm::mat4 screenProjection =
        glm::ortho(0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
                   0.0f, 1.0f);

    glm::mat4 screenMvp = screenProjection * camera.getView();

    spriteBatch.begin();
    buildSpriteVertexData(worldSprites);
    buildSpriteVertexData(actorSprites);
    spriteBatch.render(screenMvp);
    spriteBatch.end();
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