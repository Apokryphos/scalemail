#pragma once

#include <glm/vec4.hpp>

namespace ScaleMail
{
class Camera;
class Game;
class GameState;
class World;

void render(Game& game, World& world, Camera& camera, GameState& gameState,
			float totalElapsedSeconds);
}