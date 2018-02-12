#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
class Player;
class PhysicsSystem;

std::vector<Player*> getPlayersInRange(
	const std::vector<Player*>& players,
	PhysicsSystem& physicsSystem,
	glm::vec2 position,
	float distance);
}