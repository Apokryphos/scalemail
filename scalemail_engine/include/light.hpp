#pragma once

namespace ScaleMail
{
class AssetManager;
class Camera;
struct GameWindow;
class LightSystem;
struct RenderOptions;

void initializeLight(AssetManager& assetManager,
					 const RenderOptions& renderOptions);
void renderLight(GameWindow& gameWindow, Camera& camera,
				 LightSystem& lightSystem);
}