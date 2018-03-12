#pragma once

namespace ScaleMail
{
class AssetManager;
class Camera;
struct GameWindow;
class LightSystem;

void initializeLight(AssetManager& assetManager);
void renderLight(GameWindow& gameWindow, Camera& camera,
				 LightSystem& lightSystem);
}