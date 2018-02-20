#pragma once

#include <glm/vec2.hpp>

namespace ScaleMail
{
struct BulletData;
struct Entity;
struct LightData;
class World;

Entity createBullet(World& world, const Entity& sourceEntity,
					const glm::vec2& position, const BulletData& bulletData,
					const LightData& lightData);
}