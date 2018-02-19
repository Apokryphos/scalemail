#include "collision.hpp"
#include "collision_test.hpp"
#include "math_util.hpp"
#include <glm/gtx/norm.hpp>
#include <algorithm>

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
	std::vector<EntityCollision>& entityCollisions,
	bool allowPush) {
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

			bool collisionX = false;
			bool collisionY = false;

			//	Test X-axis
			if (circleIntersects(
				test.position + deltaX, test.radius,
				other.position, other.radius)) {
				result.velocity.x = 0.0f;
				result.distance = glm::distance2(test.position, other.position);
				collisionX = true;
			}

			//	Test Y-axis
			if (circleIntersects(
				test.position + deltaY, test.radius,
				other.position, other.radius)) {
				result.velocity.y = 0.0f;
				result.distance = glm::distance2(test.position, other.position);
				collisionY = true;
			}

			//	Push to slide around circle
			if (allowPush) {
				if (collisionX && !collisionY) {

					glm::vec2 push(0.0f);
					float speed = glm::length(test.velocity);

					if (test.position.y > other.position.y) {
						push.y = speed;
					} else {
						push.y = -speed;
					}

					if (!circleIntersects(
						test.position + push, test.radius,
						other.position, other.radius)) {
						result.velocity = push;
						result.push = true;
					}
				} else if (!collisionX && collisionY) {
					glm::vec2 push(0.0f);
					float speed = glm::length(test.velocity);

					if (test.position.x > other.position.x) {
						push.x = speed;
					} else {
						push.x = -speed;
					}

					if (!circleIntersects(
						test.position + push, test.radius,
						other.position, other.radius)) {
						result.velocity = push;
						result.push = true;
					}
				}
			}

			//	Add collision to results
			if (collisionX || collisionY) {
				results.push_back(result);
			}
		}

		if (results.size() > 0) {
			//	Sort results by distance, farthest first
			std::sort(results.begin(), results.end(), sortEntityCollision);

			for (auto& result : results) {
				result.sourceEntity = test.entity;
				result.sourceGroup = test.group;
				entityCollisions.push_back(result);
			}
		}

		results.resize(0);
	}
}

//	============================================================================
void processStaticCollisions(
	std::vector<CollisionTest>& tests,
	const std::vector<glm::vec4>& obstacles,
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
		}
	}
}
}