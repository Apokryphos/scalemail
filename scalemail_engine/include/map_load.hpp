#pragma once

#include <memory>
#include <string>

namespace ScaleMail
{
class Map;
class World;

std::shared_ptr<Map> loadMap(const std::string filename, World& world);
}