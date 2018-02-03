#pragma once

namespace ScaleMail
{
	class AssetManager;
	class Camera;
	class Map;

	void initializeMapMesh(AssetManager& assetManager);
	void renderMap(const Map& map, const Camera& camera, 
				   float totalElapsedSeconds);
	void updateMapMesh(float elapsedSeconds);
}