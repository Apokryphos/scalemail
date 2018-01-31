#pragma once

namespace ScaleMail
{
	class AssetManager;
	class Camera;
	struct GameWindow;
	class Map;

	void initializeMapMesh(AssetManager& assetManager);
	void renderMap(GameWindow& gameWindow, const Map& map, const Camera& camera,
				   float totalElapsedSeconds);
	void updateMapMesh(float elapsedSeconds);
}