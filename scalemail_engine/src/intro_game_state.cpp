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
#include "player_camera.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ScaleMail
{
static const std::string INTRO_CAMERA_NAME = "IntroCamera";
static const std::string INTRO_CAMERA_PATH_NAME = "IntroCameraPath";
static const std::string INTRO_DOOR_NAME = "introDoor";
static const std::string INTRO_SKELETON_NAME = "Skeleton";
static const std::string INTRO_TITLE = "- SCALEMAIL -";

const float STATE1_DURATION = 0.5f;
const float STATE2_DURATION = 2.0f;
const float STATE3_DURATION = 3.0f;
const float STATE4_DURATION = 15.0f;
const float STATE5_DURATION = 1.0f;
const float STATE5_SKIP_DURATION = 5.0f;

//	============================================================================
static bool closeDoors(World& world, std::vector<Entity>& entities,
					   const glm::vec2& cameraPosition) {
	bool closed = false;

	for (const auto entity : entities) {
		PhysicsSystem& physicsSystem = world.getPhysicsSystem();
		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 doorPosition = physicsSystem.getPosition(physicsCmpnt);

		if (cameraPosition.y >= doorPosition.y) {
			closed = true;
			DoorComponent doorCmpnt = world.getDoorSystem().getComponent(entity);
			world.getDoorSystem().setOpen(doorCmpnt, false);
		}
	}

	return closed;
}

//	============================================================================
static Camera* getIntroCamera(World& world) {
	auto entities =  world.getNameSystem().getEntitiesByName(INTRO_CAMERA_NAME);

	if (entities.size() == 0) {
		return nullptr;
	}

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt = cameraSystem.getComponent(entities[0]);

	return &(cameraSystem.getCamera(cameraCmpnt));
}

//	============================================================================
static void initializeIntroCameraPan(World& world) {
	auto entities =  world.getNameSystem().getEntitiesByName(INTRO_CAMERA_NAME);

	if (entities.size() == 0) {
		return;
	}

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt = cameraSystem.getComponent(entities[0]);

	//	Initialize camera position to path start
	cameraSystem.setMode(cameraCmpnt, CameraMode::FOLLOW_PATH);

	//	Change to fixed mode so camera remains stationary for now
	cameraSystem.setMode(cameraCmpnt, CameraMode::FIXED);
}

//	============================================================================
static void raiseSkeletons(World& world, std::vector<Entity>& entities) {
	if (entities.size() == 0) {
		return;
	}

		//	Unearth skeletons
	BurySystem& burySystem = world.getBurySystem();
	for (const auto entity : entities) {
		if (burySystem.hasComponent(entity)) {
			BuryComponent buryCmpnt = burySystem.getComponent(entity);

			if (burySystem.getBuryState(buryCmpnt) == BuryState::BURIED) {
				world.getBurySystem().rise(buryCmpnt, false);
			}
		}
	}
}

//	============================================================================
static void startIntroCameraPan(World& world) {
	auto entities =  world.getNameSystem().getEntitiesByName(INTRO_CAMERA_NAME);

	if (entities.size() == 0) {
		return;
	}

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt = cameraSystem.getComponent(entities[0]);

	cameraSystem.setMode(cameraCmpnt, CameraMode::FOLLOW_PATH);
}

//	============================================================================
static void stopIntroCameraPan(World& world) {
	auto entities =  world.getNameSystem().getEntitiesByName(INTRO_CAMERA_NAME);

	if (entities.size() == 0) {
		return;
	}

	CameraSystem& cameraSystem = world.getCameraSystem();

	CameraComponent cameraCmpnt = cameraSystem.getComponent(entities[0]);

	cameraSystem.setMode(cameraCmpnt, CameraMode::FIXED);
}

//	============================================================================
IntroGameState::IntroGameState(GameStateManager& gameStateManager) :
	GameState(gameStateManager), mSkipIntro(false), mDoorsClosed(false),
	mIntroState(0),	mIntroTicks(0.0f), mTextAlpha(0.0f) {
}

//	============================================================================
void IntroGameState::activate(Game& game) {
	World& world = *game.world;

	NameSystem& nameSystem = world.getNameSystem();
	mDoorEntities = nameSystem.getEntitiesByName(INTRO_DOOR_NAME);
	mBuriedEntities = nameSystem.getEntitiesByName(INTRO_SKELETON_NAME);

	game.gameWindow.setCursorVisible(false);

	if (!mSkipIntro) {
		setTransitionState(TransitionState::FADED_OUT);

		//	Activate intro camera
		game.camera = getIntroCamera(world);
	} else {
		//	Set state to last state
		mIntroState = 4;

		//	Use bottom of map as camera position for triggering intro entities
		const glm::vec2 mapBottom(
			0,
			world.getMap()->getHeight() * world.getMap()->getTileHeight());

		closeDoors(world, mDoorEntities, mapBottom);
		raiseSkeletons(world, mBuriedEntities);

		//	Activate player camera
		activatePlayerCamera(game);
	}

	//	Disable AI during camera pan
	world.getAiSystem().enable(false);
}

//	============================================================================
void IntroGameState::activate(Game& game, bool skipIntro) {
	mSkipIntro = skipIntro;
	this->activate(game);
}

//	============================================================================
void IntroGameState::draw(const Game& game) {
	const GameWindow& gameWindow = game.gameWindow;

	const float textSize = NORMAL_FONT_SIZE * game.camera->getZoom();
	const float centerX = gameWindow.getWidth() * 0.5f;
	const float centerY = gameWindow.getHeight() * 0.5f - textSize;

	if (!game.paused) {
		drawCenterText(
			glm::vec2(centerX, centerY),
			INTRO_TITLE,
			glm::vec4(1.0f, 1.0f, 1.0f, mTextAlpha),
			textSize);
	}
}

//	============================================================================
void IntroGameState::initialize(Game& game) {
	World& world = *game.world;

	Entity introCamera = createCamera(world, INTRO_CAMERA_NAME);
	CameraSystem& cameraSystem = world.getCameraSystem();
	CameraComponent cameraCmpnt = cameraSystem.getComponent(introCamera);
	cameraSystem.setPath(cameraCmpnt, INTRO_CAMERA_PATH_NAME);

	createPlayerCamera(world);
}

//	============================================================================
void IntroGameState::update(Game& game, float elapsedSeconds) {
	this->updateState(*game.world, elapsedSeconds);
}

//	============================================================================
void IntroGameState::updateState(World& world, float elapsedSeconds) {
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

		glm::vec2 cameraPosition(0.0f);

		Camera* camera = getIntroCamera(world);
		if (camera != nullptr) {
			cameraPosition = camera->getPosition();
		}

		//	Close doors as camera Y position passes them
		if (!mDoorsClosed) {
			mDoorsClosed = closeDoors(world, mDoorEntities, cameraPosition);

			if (mDoorsClosed) {
				raiseSkeletons(world, mBuriedEntities);
			}
		}

		if (mIntroTicks >= STATE4_DURATION) {
			mIntroTicks = 0;
			stopIntroCameraPan(world);
			++mIntroState;
		}
		break;
	}

	//  Pause before activating next game state
	case 4:
		//	Use extended duration if the intro is being skipped
		const float duration =
			mSkipIntro ? STATE5_SKIP_DURATION : STATE5_DURATION;

		mIntroTicks += elapsedSeconds;
		if (mIntroTicks >= duration) {
			mIntroTicks = 0;
			++mIntroState;
			this->getGameStateManager().activateMainGameState();
		}
	}
}
}
