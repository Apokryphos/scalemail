#include "camera.hpp"
#include "door_system.hpp"
#include "ease.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "game_window.hpp"
#include "intro_game_state.hpp"
#include "map.hpp"
#include "name_system.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ScaleMail
{
const float STATE1_DURATION = 0.5f;
const float STATE2_DURATION = 2.0f;
const float STATE3_DURATION = 3.0f;
const float STATE4_DURATION = 15.0f;
const float STATE5_DURATION = 1.0f;

//	============================================================================
IntroGameState::IntroGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager), mDoorsClosed(false), mIntroState(0),
	mIntroTicks(0.0f), mTextAlpha(0.0f), mCameraStartY(0.0f),
	mCameraEndY(0.0f) {
}

//	============================================================================
void IntroGameState::activate(Game& game) {
	setTransitionState(TransitionState::FADED_OUT);

	const MapCamera* introCamera =
		game.world->getMap()->getCamera("IntroCamera");

	assert(introCamera != nullptr);
	assert(introCamera->bounds.size() == 1);
	assert(introCamera->paths.size() == 1);

	const Rectangle& bounds = introCamera->bounds[0];

	//	Camera path should stretch from top to bottom of bounds
	const MapCameraPath& path = introCamera->paths[0];

	assert(path.points.size() == 2);

	Camera& camera = *game.camera;

	//	Set bounds before position so camera position will be
	//	adjusted to fit in bounds
	camera.setBounds(bounds);

	//	Use bounds adjusted camera position for end position
	camera.setPosition(glm::vec2(path.points[1].y, path.points[1].y));
	mCameraEndY = camera.getPosition().y;

	//	Use bounds adjusted camera position for start
	//	This will also set the camera to the correct initial position
	camera.setPosition(glm::vec2(path.points[0].x, path.points[0].y));
	mCameraStartY = camera.getPosition().y;

	World& world = *game.world;
	NameSystem& nameSystem = world.getNameSystem();
	mDoorEntities = nameSystem.getEntitiesByName("introDoor");
	mBuriedEntities = nameSystem.getEntitiesByName("Skeleton");

	//	Disable AI during camera pan
	world.getAiSystem().enable(false);
}

//	============================================================================
void IntroGameState::draw(const Game& game, Camera& camera) {
	const GameWindow& gameWindow = game.gameWindow;

	const float textSize = NORMAL_FONT_SIZE * camera.getZoom();
	const float centerX = gameWindow.width * 0.5f;
	const float centerY = gameWindow.height * 0.5f - textSize;

	if (!game.paused) {
		drawCenterText(
			glm::vec2(centerX, centerY),
			"- SCALEMAIL -",
			glm::vec4(1.0f, 1.0f, 1.0f, mTextAlpha),
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
	switch (mIntroState) {
	//  Pause before fading title in
	case 0:
		mIntroTicks += elapsedSeconds;
		if (mIntroTicks >= STATE1_DURATION) {
			mIntroTicks = 0;
			++mIntroState;
		}
		break;

	//  Fade title in
	case 1:
		mIntroTicks += elapsedSeconds;
		mTextAlpha = easeInCubic(mIntroTicks, 0, 1, STATE2_DURATION);
		if (mIntroTicks >= STATE2_DURATION) {
			mIntroTicks = 0;
			transitionFadeIn();
			++mIntroState;
		}
		break;

	//  Pause after fading in map
	case 2:
		mIntroTicks += elapsedSeconds;
		mTextAlpha = 1.0f;
		if (mIntroTicks >= STATE3_DURATION) {
			mIntroTicks = 0;
			++mIntroState;
		}
		break;

	//  Scroll down and fade out title
	case 3:
	{
		mIntroTicks += elapsedSeconds;
		mTextAlpha = 1 - easeOutCubic(mIntroTicks, 0, 1, STATE2_DURATION);

		glm::vec2 position;

		position.y = easeInOutSine(
			mIntroTicks,
			mCameraStartY,
			mCameraEndY - mCameraStartY,
			STATE4_DURATION);

		//	Close doors as camera Y position passes them
		if (!mDoorsClosed) {
			for (const auto entity : mDoorEntities) {
				PhysicsSystem& physicsSystem = world.getPhysicsSystem();
				PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
				glm::vec2 doorPosition = physicsSystem.getPosition(physicsCmpnt);

				if (position.y >= doorPosition.y) {
					mDoorsClosed = true;
					DoorComponent doorCmpnt = world.getDoorSystem().getComponent(entity);
					world.getDoorSystem().setOpen(doorCmpnt, false);
				}
			}

			if (mDoorsClosed) {
				//	Unearth skeletons
				for (const auto entity : mBuriedEntities) {
					BurySystem& burySystem = world.getBurySystem();

					if (burySystem.hasComponent(entity)) {
						BuryComponent buryCmpnt = burySystem.getComponent(entity);
						world.getBurySystem().rise(buryCmpnt, false);
					}
				}
			}
		}

		camera.setPosition(position);

		if (mIntroTicks >= STATE4_DURATION) {
			mIntroTicks = 0;
			camera.setPosition(glm::vec2(0, mCameraEndY));
			++mIntroState;
		}
		break;
	}

	//  Pause before activating next game state
	case 4:
		mIntroTicks += elapsedSeconds;
		if (mIntroTicks >= STATE5_DURATION) {
			mIntroTicks = 0;
			++mIntroState;
			this->getGameStateManager().activateMainGameState();
		}
	}
}
}
