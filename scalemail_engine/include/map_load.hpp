#pragma once

#include <memory>
#include <string>

namespace ScaleMail
{
class Map;
struct RenderOptions;
class World;

std::shared_ptr<Map> loadMap(const std::string filename, World& world,
							 const RenderOptions& renderOptions);
}