#pragma once

#include "entity.hpp"
#include <string>

namespace ScaleMail
{
class World;

class PrefabFactory
{
public:
	PrefabFactory() = default;
	PrefabFactory(const PrefabFactory&) = delete;
	PrefabFactory& operator=(const PrefabFactory&) = delete;
	void buildPrefab(Entity entity, std::string prefabName, World& world);
};
}