#pragma once

#include "direction.hpp"
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

Entity createLoot(World& world, const glm::vec2& position,
				  const glm::vec2& size, const int tilesetId,
				  const std::string& name, const std::string& prefab);

Entity createPlayerActor(World& world, const glm::vec2 position,
						 const int actorIndex, const Direction facing,
						 const std::string& name);

Entity createVillainActor(World& world, const glm::vec2& position,
						  const glm::vec2& size, const int actorIndex,
						  const Direction facing, const std::string& name,
						  const std::string& prefab, const std::string& ai);

Entity createProp(World& world, const glm::vec2& position, const int tilesetId,
				  const bool decal);

Entity createProp(World& world, const glm::vec2& position,
				  const int frame1TilesetId, int frame2TilesetId, bool decal);

Entity createTrigger(World& world, const glm::vec2& position,
					 const glm::vec2& size, const std::string& targetName);

void destroyBullet(World& world, const Entity& entity);
}