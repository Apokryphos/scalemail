#include "ai_system.hpp"
#include "bury_system.hpp"
#include "camera.hpp"
#include "camera_system.hpp"
#include "door_system.hpp"
#include "ease.hpp"
#include "entity_types.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state_manager.hpp"
#include "game_window.hpp"
#include "intro_game_state.hpp"
#include "map.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
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
void getIntroCamera(World& world, Entity& cameraEntity, Camera*& camera) {
	camera = nullptr;

	NameSystem& nameSystem = world.getNameSystem();

	auto cameraEntities = nameSystem.getEntitiesByName("IntroCamera");

	//	Intro camera
	if (cameraEntities.size() > 0) {
		cameraEntity = cameraEntities[0];

		CameraSystem& cameraSystem = world.getCameraSystem();

		const CameraComponent cameraCmpnt =
			cameraSystem.getComponent(cameraEntity);

		camera = &(cameraSystem.getCamera(cameraCmpnt));
	}
}

//	============================================================================
static void initializeIntroCameraPan(World& world) {
	Camera* camera;
	Entity cameraEntity;
	getIntroCamera(world, cameraEntity, camera);

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt =
		cameraSystem.getComponent(cameraEntity);

	cameraSystem.setMode(cameraCmpnt, CameraMode::FOLLOW_PATH);

	cameraSystem.setMode(cameraCmpnt, CameraMode::FIXED);
}

//	============================================================================
static void startIntroCameraPan(World& world) {
	Camera* camera;
	Entity cameraEntity;
	getIntroCamera(world, cameraEntity, camera);

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt =
		cameraSystem.getComponent(cameraEntity);

	cameraSystem.setMode(cameraCmpnt, CameraMode::FOLLOW_PATH);
}

//	============================================================================
IntroGameState::IntroGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager), mDoorsClosed(false), mIntroState(0),
	mIntroTicks(0.0f), mTextAlpha(0.0f) {
}

//	============================================================================
void IntroGameState::activate(Game& game) {
	setTransitionState(TransitionState::FADED_OUT);

	World& world = *game.world;

	Camera* camera;
	Entity cameraEntity;
	getIntroCamera(world, cameraEntity, camera);
	game.camera = camera;

	NameSystem& nameSystem = world.getNameSystem();
	mDoorEntities = nameSystem.getEntitiesByName("introDoor");
	mBuriedEntities = nameSystem.getEntitiesByName("Skeleton");

	game.gameWindow.setCursorVisible(false);

	//	Disable AI during camera pan
	// world.getAiSystem().enable(false);
}

//	============================================================================
void IntroGameState::draw(const Game& game, Camera& camera) {
	const GameWindow& gameWindow = game.gameWindow;

	const float textSize = NORMAL_FONT_SIZE * camera.getZoom();
	const float centerX = gameWindow.getWidth() * 0.5f;
	const float centerY = gameWindow.getHeight() * 0.5f - textSize;

	if (!game.paused) {
		drawCenterText(
			glm::vec2(centerX, centerY),
			"- SCALEMAIL -",
			glm::vec4(1.0f, 1.0f, 1.0f, mTextAlpha),
			textSize);
	}
}

//	============================================================================
void IntroGameState::initialize(Game& game) {
	World& world = *game.world;

	Entity introCamera = createCamera(world, "IntroCamera");
	CameraSystem& cameraSystem = world.getCameraSystem();
	CameraComponent cameraCmpnt = cameraSystem.getComponent(introCamera);
	cameraSystem.setPath(cameraCmpnt, "IntroCameraPath");
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
			initializeIntroCameraPan(world);
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
			startIntroCameraPan(world);
			++mIntroState;
		}
		break;

	//  Scroll down and fade out title
	case 3:
	{
		mIntroTicks += elapsedSeconds;
		mTextAlpha = 1 - easeOutCubic(mIntroTicks, 0, 1, STATE2_DURATION);

		glm::vec2 position(0.0f);

		Camera* camera;
		Entity cameraEntity;
		getIntroCamera(world, cameraEntity, camera);

		if (camera != nullptr) {
			position = camera->getPosition();
		}

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

		if (mIntroTicks >= STATE4_DURATION) {
			mIntroTicks = 0;

			if (camera != nullptr) {
				CameraSystem& cameraSystem = world.getCameraSystem();

				CameraComponent cameraCmpnt =
					cameraSystem.getComponent(cameraEntity);

				cameraSystem.setMode(cameraCmpnt, CameraMode::FIXED);
			}

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
