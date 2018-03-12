#pragma once

#include <memory>
#include <string>

namespace ScaleMail
{
struct AssetManager;
class Map;
class World;

std::shared_ptr<Map> loadMap(const std::string filename, World& world,
							 const AssetManager& assetManager);
}