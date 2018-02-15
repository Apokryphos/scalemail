#pragma once

#include "entity.hpp"
#include "entity_collision.hpp"
#include "static_collision.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
struct CollisionTest
{
	CollisionGroup group;
	float radius;
	glm::vec2 position;
	glm::vec2 velocity;
	Entity entity;
};

void processEntityCollisions(
	const std::vector<CollisionTest>& tests,
	const std::vector<CollisionTest>& otherTests,
	std::vector<CollisionTest>& passed,
	std::vector<EntityCollision>& entityCollisions);

void processStaticCollisions(
	std::vector<CollisionTest>& tests,
	const std::vector<glm::vec4>& obstacles,
	std::vector<CollisionTest>& passed,
	std::vector<StaticCollision>& staticCollisions);
}