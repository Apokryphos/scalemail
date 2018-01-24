#include "light.hpp"
#include "sprite.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
//  ============================================================================
static void addWorldSprite(int tileX, int tileY, int tilesetId) {
    addWorldSprite(glm::vec2(tileX * 16, tileY * 16), tilesetId);
}

//  ============================================================================
static void addTorch(int tileX, int tileY) {
    const glm::vec4 torchLightColor(1.0f, 0.6f, 0.0f, 0.75f);
    float torchLightSize = 64;
    float torchLightPulse = 8;
    float torchLightPulseSize = 8;

    addLight(
        glm::vec2(tileX * 16.0f + 8.0f, tileY * 16.0f + 10.0f),
        torchLightColor,
        torchLightSize,
        torchLightPulse,
        torchLightPulseSize
    );

    addWorldSprite(glm::vec2(tileX * 16, tileY * 16), 197);
}

//  ============================================================================
static void addActorSprite(float x, float y, int actorIndex,
                           Direction facing = Direction::SOUTH) {
    addActorSprite(glm::vec2(x + 8.0f, y - 8.0f), actorIndex, facing);
}

//  ============================================================================
static void initializeFakeMapSprites() {
    addTorch(7, 0);
    addTorch(9, 0);
    addTorch(7, 8);
    addTorch(10, 8);
    addTorch(13, 19);
    addTorch(8, 26);
    addTorch(9, 37);
    addTorch(7, 40);
    addTorch(6, 48);
    addTorch(8, 48);
    addTorch(14, 55);

    addActorSprite(105.0f, 810.0f, 4);
    addActorSprite(88.0f, 805.0f, 6, Direction::EAST);
    addActorSprite(123.0f, 812.0f, 2);
    addActorSprite(137.0f, 803.0f, 1, Direction::WEST);

    addActorSprite(120.0f, 912.0f, 17, Direction::NORTH);
    addActorSprite(105.0f, 920.0f, 17, Direction::EAST);
    addActorSprite(134.0f, 919.0f, 17, Direction::NORTH);
    addActorSprite(149.0f, 924.0f, 17, Direction::WEST);

    addActorSprite(114.0f, 938.0f, 16, Direction::NORTH);
    addActorSprite(136.0f, 942.0f, 16, Direction::WEST);

    const glm::vec4 orangeShroomLightColor(0.92f, 0.53f, 0.19f, 0.10f);
    const glm::vec4 redShroomLightColor(0.92f, 0.137f, 0.137f, 0.10f);
    const glm::vec4 pinkShroomLightColor(0.77f, 0.176f, 0.77f, 0.10f);
    float shroomLightSize = 64;
    float shroomLightPulse = 2;
    float shroomLightPulseSize = 2;

    addLight(
        glm::vec2(109, 81.0f),
        redShroomLightColor,
        shroomLightSize,
        shroomLightPulse,
        shroomLightPulseSize
    );

    addLight(
        glm::vec2(176, 178.0f),
        pinkShroomLightColor,
        shroomLightSize,
        shroomLightPulse,
        shroomLightPulseSize
    );
}

//  ============================================================================
void initializeFakeMap() {
    initializeFakeMapSprites();
}
}