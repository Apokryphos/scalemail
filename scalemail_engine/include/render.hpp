#pragma once

#include <glm/vec4.hpp>

namespace ScaleMail
{
class AssetManager;
class Camera;
class Game;
class GameState;
class World;
struct RenderOptions;

void initializeRender(AssetManager& assetManager,
					  RenderOptions& renderOptions);
void render(Game& game, World& world, Camera& camera, GameState& gameState,
			float totalElapsedSeconds);
}