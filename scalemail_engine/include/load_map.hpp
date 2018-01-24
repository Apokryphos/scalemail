#pragma once

#include "map.hpp"
#include "world.hpp"
#include <memory>
#include <string>

namespace ScaleMail
{
class World;

std::shared_ptr<Map> loadMap(const std::string filename, World& world);
}