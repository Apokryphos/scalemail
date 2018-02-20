#pragma once

#include <glm/vec2.hpp>
#include <string>

namespace ScaleMail
{
struct BulletData;
struct Entity;
struct LightData;
class World;

Entity createBullet(World& world, const Entity& sourceEntity,
					const glm::vec2& position, const BulletData& bulletData,
					const LightData& lightData);

Entity createDoor(World& world, const glm::vec2& position,
				  const int openTilesetId, const int closedTilesetId,
				  const bool open, const std::string& name);

Entity createDoor(World& world, const glm::vec2& position,
				  const int openTilesetId, const int closedTilesetId,
				  const int childOpenTilesetId,
				  const int childClosedTilesetId, const bool open,
				  const std::string& name);
}