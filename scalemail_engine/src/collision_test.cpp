#include "collision.hpp"
#include "collision_test.hpp"
#include <algorithm>
#include <glm/gtx/norm.hpp>

namespace ScaleMail
{
struct EntityCollisionResult
{
	glm::vec2 velocity;
	Entity targetEntity;
	CollisionGroup targetGroup;
};

//	============================================================================
inline static bool sortEntityCollision(const EntityCollision& result1,
					   			const EntityCollision& result2) {
	return result1.distance > result2.distance;
}

//	============================================================================
void processEntityCollisions(
	const std::vector<CollisionTest>& tests,
	const std::vector<CollisionTest>& otherTests,
	std::vector<CollisionTest>& passed,
	std::vector<EntityCollision>& entityCollisions) {
	//	Separate results so they can be sorted by distance
	std::vector<EntityCollision> results;

	for (const auto& test : tests) {
		glm::vec2 deltaX = glm::vec2(test.velocity.x, 0);
		glm::vec2 deltaY = glm::vec2(0, test.velocity.y);

		for (const auto& other : otherTests) {
			//	Don't test entity against itself
			if (other.entity.id == test.entity.id) {
				continue;
			}

			EntityCollision result = {};
			result.targetEntity = other.entity;
			result.targetGroup = other.group;
			result.velocity = test.velocity;

			bool collision = false;

			//	Test X-axis
			if (circleIntersects(
				test.position + deltaX, test.radius,
				other.position, other.radius)) {
				result.velocity.x = 0.0f;
				result.distance = glm::distance2(test.position + deltaX, other.position);
				collision = true;
			}

			//	Test Y-axis
			if (circleIntersects(
				test.position + deltaY, test.radius,
				other.position, other.radius)) {
				result.velocity.y = 0.0f;
				result.distance = glm::distance2(test.position + deltaY, other.position);
				collision = true;
			}

			if (collision) {
				results.push_back(result);
			}
		}

		if (results.size() > 0) {
			//	Sort results by distance
			std::sort(results.begin(), results.end(), sortEntityCollision);

			//	Use nearest result
			EntityCollision result = results[0];

			result.sourceEntity = test.entity;
			result.sourceGroup = test.group;
			entityCollisions.push_back(result);
		} else {
			passed.push_back(test);
		}

		results.resize(0);
	}
}

//	============================================================================
void processStaticCollisions(
	std::vector<CollisionTest>& tests,
	const std::vector<glm::vec4>& obstacles,
	std::vector<CollisionTest>& passed,
	std::vector<StaticCollision>& staticCollisions) {
	for (auto& test : tests) {
		glm::vec2 deltaX = glm::vec2(test.velocity.x, 0);
		glm::vec2 deltaY = glm::vec2(0, test.velocity.y);

		bool collision = false;

		for (const auto& obstacle : obstacles) {
			if (circleIntersectsRectangle(
				test.position + deltaX, test.radius, obstacle)) {
				test.velocity.x = 0.0f;
				collision = true;
				break;
			}
		}

		for (const auto& obstacle : obstacles) {
			if (circleIntersectsRectangle(
				test.position + deltaY, test.radius, obstacle)) {
				test.velocity.y = 0.0f;
				collision = true;
				break;
			}
		}

		if (collision) {
			StaticCollision staticCollision = {};
			staticCollision.sourceEntity = test.entity;
			staticCollision.sourceGroup = test.group;
			staticCollision.velocity = test.velocity;
			staticCollisions.push_back(staticCollision);
		} else {
			passed.push_back(test);
		}
	}
}
}