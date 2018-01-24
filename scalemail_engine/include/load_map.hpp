#pragma once

#include "map.hpp"
#include <memory>
#include <string>

namespace ScaleMail
{
std::shared_ptr<Map> loadMap(const std::string filename);
}