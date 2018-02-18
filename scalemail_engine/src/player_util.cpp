#include "player.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>

namespace ScaleMail
{
//  ============================================================================
std::vector<Player*> getPlayersInRange(
	const std::vector<Player*>& players,
	PhysicsSystem& physicsSystem,
	glm::vec2 position,
	float distance) {
	std::vector<Player*> inRange;

	const float distanceSquared = distance * distance;

	for (Player* player : players) {
		if (physicsSystem.hasComponent(player->entity)) {
			PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(player->entity);

			glm::vec2 playerPosition =
				physicsSystem.getPosition(physicsCmpnt);

			if (glm::distance2(position, playerPosition) <= distanceSquared) {
				inRange.push_back(player);
			}
		}
	}

	return inRange;
}

//  ============================================================================
Player* getRandomPlayerInRange(World& world, const glm::vec2& position,
							   float range) {
	std::vector<Player*> allPlayers = world.getPlayers();

	std::vector<Player*> players =
		getPlayersInRange(
			allPlayers, world.getPhysicsSystem(), position, range);

	return world.getRandom().getRandomElement(players);
}
}