#pragma once

#include "entity.hpp"
#include <string>

namespace ScaleMail
{
class World;

class PrefabFactory
{
public:
	void buildPrefab(Entity entity, std::string prefabName, World& world);
};
}