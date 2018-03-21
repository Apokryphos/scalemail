#include "game_state_manager.hpp"
#include <iostream>

namespace ScaleMail
{
//	============================================================================
GameStateManager::GameStateManager() : mIntroGameState(*this),
									   mMainGameState(*this) {
	mActiveGameState = nullptr;
}

//	============================================================================
void GameStateManager::activateIntroGameState(bool skipIntro) {
	mActiveGameState = &mIntroGameState;
	mIntroGameState.activate(*mGame, skipIntro);
	std::cout << "Intro game state activated." << std::endl;
}

//	============================================================================
void GameStateManager::activateMainGameState() {
	mActiveGameState = &mMainGameState;
	mMainGameState.activate(*mGame);
	std::cout << "Main game state activated." << std::endl;
}

//	============================================================================
GameState* GameStateManager::getActiveGameState() {
	return mActiveGameState;
}

//	============================================================================
void GameStateManager::initialize(Game& game) {
	mGame = &game;
	mIntroGameState.initialize(game);
	mMainGameState.initialize(game);
}
}