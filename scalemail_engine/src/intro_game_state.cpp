#include "camera.hpp"
#include "door_system.hpp"
#include "ease.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "intro_game_state.hpp"
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

const float introCameraStartY = 128.0f;
const float introCameraEndY = 1152.0f;

static std::vector<Entity> doorEntities;

//	============================================================================
IntroGameState::IntroGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager) {
	introState = 0;
	introTicks = 0;
	textAlpha = 0.0f;
}

//	============================================================================
void IntroGameState::activate(Game& game) {
	setTransitionState(TransitionState::FADED_OUT);

	Camera& camera = *game.camera;
	camera.position = glm::vec2(128.0f, introCameraStartY);

	World& world = *game.world;
	doorEntities = world.getEntitiesByName("introDoor");
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
void IntroGameState::initialize([[maybe_unused]] Game& game) {
}

//	============================================================================
void IntroGameState::update(Game& game, float elapsedSeconds) {
	this->updateState(*game.world, *game.camera, elapsedSeconds);
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
			++introState;
			this->getGameStateManager().activateMainGameState();
		}
	}
}
}
