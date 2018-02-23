#pragma once

namespace ScaleMail
{
class AssetManager;
class Camera;
class Map;

void initializeMapMesh(AssetManager& assetManager);
void renderMap(const Map& map, const Camera& camera, double totalElapsedSeconds);
void updateMapMeshAnimation(float elapsedSeconds);
}