#include "camera.hpp"
#include "door_system.hpp"
#include "ease.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "intro_game_state.hpp"
#include "light_system.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

namespace ScaleMail
{
const float STATE1_DURATION = 0.5f;
const float STATE2_DURATION = 2.0f;
const float STATE3_DURATION = 3.0f;
const float STATE4_DURATION = 15.0f;
const float STATE5_DURATION = 1.0f;

const float introCameraStartY = 0.0f;
const float introCameraEndY = -1024.0f;

static const glm::vec4 startAmbientColor(0.14f, 0.064f, 0.04f, 1.0f);
static const glm::vec4 endAmbientColor(0.3f, 0.38f, 0.4f, 1.0f);

static std::vector<Entity> doorEntities;

//	============================================================================
IntroGameState::IntroGameState() {
    introState = 0;
    introTicks = 0;
    textAlpha = 0.0f;
}

//	============================================================================
void IntroGameState::draw(const Game& game, Camera& camera) {
    const GameWindow& gameWindow = game.gameWindow;

    const float textSize = 8.0f * camera.zoom;
    const float centerX = gameWindow.width * 0.5f;
    const float centerY = gameWindow.height * 0.5f - textSize;

    if (game.paused) {
        drawCenterText(
            glm::vec2(centerX, centerY),
            "PAUSED",
            glm::vec4(1.0f),
            textSize);
    } else {
        drawCenterText(
            glm::vec2(centerX, centerY),
            "- SCALEMAIL -",
            glm::vec4(1.0f, 1.0f, 1.0f, textAlpha),
            textSize);
    }
}

//	============================================================================
void IntroGameState::initialize(World& world, Camera& camera) {
    setTransitionState(TransitionState::FADED_OUT);
    world.getLightSystem().setAmbientColor(startAmbientColor);
    camera.position = glm::vec2(0, introCameraStartY);

    doorEntities = world.getEntitiesByName("introDoor");
}

//	============================================================================
void IntroGameState::update(World& world, Camera& camera, float elapsedSeconds) {
    this->updateState(world, camera, elapsedSeconds);
}

//	============================================================================
void IntroGameState::updateState(World& world, Camera& camera,
                                 float elapsedSeconds) {
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
    {
        introTicks += elapsedSeconds;
        textAlpha = 1 - easeOutCubic(introTicks, 0, 1, STATE2_DURATION);

        glm::vec4 ambientColor;

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

        world.getLightSystem().setAmbientColor(ambientColor);

        camera.position.y = easeInOutSine(
            introTicks,
            introCameraStartY,
            introCameraEndY - introCameraStartY,
            STATE4_DURATION);

        if (camera.position.y < introCameraEndY + 64) {
            for (auto entity : doorEntities) {
                DoorComponent doorCmpnt = world.getDoorSystem().getComponent(entity);
                world.getDoorSystem().setOpen(doorCmpnt, false);
            }
        }

        if (introTicks >= STATE4_DURATION) {
            introTicks = 0;
            camera.position.y = introCameraEndY;
            ++introState;
        }
        break;
    }

    //  Pause before fading out
    case 4:
        introTicks += elapsedSeconds;
        if (introTicks >= STATE5_DURATION) {
            introTicks = 0;
            // transitionFadeOut();
            ++introState;
        }
    }
}
}
