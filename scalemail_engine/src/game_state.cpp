#include "game_state.hpp"

namespace ScaleMail
{
//	============================================================================
GameState::GameState(GameStateManager& gameStateManager) :
	mGameStateManager(gameStateManager) {
}

//	============================================================================
GameStateManager& GameState::getGameStateManager() {
	return mGameStateManager;
}
}