#include "ai_behavior.hpp"
#include "ai_system.hpp"
#include "gl_headers.hpp"
#include "vector_util.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/mat4x4.hpp>
#include <iostream>

namespace ScaleMail
{
static const float OBSTACLE_SCALE = 1.5f;

//	============================================================================
static inline void addLineVertexData(
	std::vector<float>& vertexData,
	const glm::vec2& position,
	const glm::vec2& direction,
	const glm::vec4& color) {
	const float VECTOR_LINE_LENGTH = 24.0f;

	vertexData.emplace_back(position.x);
	vertexData.emplace_back(position.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);

	const glm::vec2 end =
		position + normalizeVec2(direction) * VECTOR_LINE_LENGTH;

	vertexData.emplace_back(end.x);
	vertexData.emplace_back(end.y);
	vertexData.emplace_back(color.r);
	vertexData.emplace_back(color.g);
	vertexData.emplace_back(color.b);
	vertexData.emplace_back(color.a);
}

//	============================================================================
static AiComponent makeComponent(const int index) {
	return AiComponent(index);
}

//	============================================================================
AiSystem::AiSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents), mEnabled(true) {
	mData.reserve(maxComponents);
}

//	============================================================================
void AiSystem::addBehavior(const AiComponent& cmpnt,
						   std::shared_ptr<AiBehavior> behavior) {
	mData[cmpnt.index].behaviors.push_back(behavior);
}

//	============================================================================
void AiSystem::addObstacle(const glm::vec2& position, const float radius) {
	Obstacle obstacle = {};
	obstacle.position = position;
	obstacle.radius = radius;

	mObstacles.push_back(obstacle);
}

//	============================================================================
void AiSystem::addObstacle(const float x, const float y, const float width,
						   const float height) {
	const float MAX_RADIUS = 8.0f;

	//	Square and less than max radius
	if (width == height && width * 0.5f <= MAX_RADIUS) {
		const float radius = width * 0.5f;
		this->addObstacle(glm::vec2(x + radius, y + radius), radius);
		return;
	}

	//	Use smallest radius
	const float radius =
		std::min(std::min(width * 0.5f, height * 0.5f), MAX_RADIUS);

	const int xCount = std::floor(width / (radius * 2.0f));
	const int yCount = std::floor(height / (radius * 2.0f));

	for (int cy = 0; cy < yCount; ++cy) {
		for (int cx = 0; cx < xCount; ++cx) {
			this->addObstacle(
				glm::vec2(
					x + radius + cx * radius * 2.0f,
					y + radius + cy * radius * 2.0f),
				radius);
		}
	}

	bool xPartial = xCount < width / (radius * 2.0f);
	bool yPartial = yCount < height / (radius * 2.0f);

	//	Add any circles for remaining space along right side
	if (xPartial) {
		for (int cy = 0; cy < yCount; ++cy) {
			this->addObstacle(
				glm::vec2(
					x + width - radius,
					y + radius + cy * radius * 2.0f),
				radius);
		}
	}

	//	Add any circles for remaining space along bottom
	if (yPartial) {
		for (int cx = 0; cx < xCount; ++cx) {
			this->addObstacle(
				glm::vec2(
					x + radius + cx * radius * 2.0f,
					y + height - radius),
				radius);
		}
	}

	//	Add circle for remaining space in bottom right corner
	if (xPartial && yPartial) {
		this->addObstacle(
			glm::vec2(x + width - radius, y + height - radius),
			radius);
	}
}

//	============================================================================
void AiSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void AiSystem::drawDebug(std::vector<float>& lineVertexData) {
	const int lineCount = 16;
	const glm::vec4 obstacleColor = glm::vec4(0.25f, 0.75f, 1.0f, 1.0f);
	const glm::vec4 obstacleScaledColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

	for (const auto& obstacle : mObstacles) {
		addCircleVertexData(
			lineVertexData,
			lineCount,
			obstacle.position,
			obstacle.radius,
			obstacleColor);

		addCircleVertexData(
			lineVertexData,
			lineCount,
			obstacle.position,
			obstacle.radius * OBSTACLE_SCALE,
			obstacleScaledColor);
	}

	const glm::vec4 avoidColor(1.0f, 0.0f, 0.0f, 1.f);
	const glm::vec4 moveColor(0.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 seekColor(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 wanderColor(1.0f, 1.0f, 1.0f, 1.0f);

	for (size_t n = 0; n < mData.size(); ++n) {
		const AiComponentData& data = mData[n];

		addLineVertexData(lineVertexData, data.position, data.avoidForce, avoidColor);
		addLineVertexData(lineVertexData, data.position, data.moveDirection, moveColor);
		addLineVertexData(lineVertexData, data.position, data.seekForce, seekColor);
		addLineVertexData(lineVertexData, data.position, data.wanderForce, wanderColor);
	}
}

//	============================================================================
void AiSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
void AiSystem::enable(bool enabled) {
	mEnabled = enabled;
}

//	============================================================================
AiComponent AiSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
void AiSystem::setMoveDirection(const AiComponent& cmpnt, glm::vec2 direction) {
	mData[cmpnt.index].moveDirection = direction;
}

//	============================================================================
void AiSystem::setSeek(const AiComponent& cmpnt, bool enabled) {
	mData[cmpnt.index].seekEnabled = enabled;
}

//	============================================================================
void AiSystem::setSeekTarget(const AiComponent& cmpnt, const glm::vec2& target) {
	mData[cmpnt.index].seekTarget = target;
}

//	============================================================================
void AiSystem::setWander(const AiComponent& cmpnt, bool enabled) {
	mData[cmpnt.index].wanderEnabled = enabled;
}

//	============================================================================
void AiSystem::update(World& world, float elapsedSeconds) {
	if (!mEnabled) {
		return;
	}

	//	Update AI behaviors
	size_t count = mData.size();
	for (size_t index = 0; index < count; ++index) {
		for (auto& behavior : mData[index].behaviors) {
			behavior.get()->think(world, elapsedSeconds);
		}
	}

	Random& random = world.getRandom();
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	for (auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);

		//	Calculate wander forces
		if (mData[index].wanderEnabled) {
			glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);
			glm::vec2 velocity = physicsSystem.getVelocity(physicsCmpnt);

			const float WANDER_CIRCLE_DISTANCE = 4.0f;

			glm::vec2 circleCenter =
				normalizeVec2(position + velocity) * WANDER_CIRCLE_DISTANCE;

			const float WANDER_ANGLE_CHANGE = glm::radians(15.0f);

			mData[index].wanderAngle += random.nextFloat(
				-WANDER_ANGLE_CHANGE * 0.5f,
				 WANDER_ANGLE_CHANGE * 0.5f);

			const float WANDER_CIRCLE_RADIUS = 8.0f;

			//	Pick random direction
			glm::vec2 displacement =
				rotateVec2(
					glm::vec2(WANDER_CIRCLE_RADIUS, 0.0f),
					mData[index].wanderAngle);

			mData[index].wanderForce = circleCenter + displacement;

			physicsSystem.addForce(physicsCmpnt, mData[index].wanderForce);
		}

		//	Calculate seek forces
		if (mData[index].seekEnabled) {
			float speed = physicsSystem.getSpeed(physicsCmpnt);
			glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);
			glm::vec2 velocity = physicsSystem.getVelocity(physicsCmpnt);

			glm::vec2 v =
				normalizeVec2(mData[index].seekTarget - position) * speed;

			mData[index].seekForce = v - velocity;

			physicsSystem.addForce(physicsCmpnt, mData[index].seekForce);
		}
	}

	//	Amount to scale obstacle radius for purpose of avoidance
	const float AVOID_MAX_FORCE = 64.0f;

	//	Distance ahead of character to detect obstacles
	const float AVOID_DISTANCE = 16.0f;

	for (auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);
		glm::vec2 velocity = physicsSystem.getVelocity(physicsCmpnt);

		//	Save position for debugging
		mData[index].position = position;

		glm::vec2 ahead = position + normalizeVec2(velocity) * AVOID_DISTANCE;

		glm::vec2 ahead2 = position + normalizeVec2(velocity) * AVOID_DISTANCE * 0.5f;

		bool collision = false;

		//	Avoid obstacles
		glm::vec2 avoid(0.0f);
		const size_t obstacleCount = mObstacles.size();
		for (size_t index = 0; index < obstacleCount; ++index) {
			const float radius =
				(mObstacles[index].radius * OBSTACLE_SCALE) *
				(mObstacles[index].radius * OBSTACLE_SCALE);

			const float distance = glm::length2(ahead - mObstacles[index].position);
			const float distance2 = glm::length2(ahead2 - mObstacles[index].position);

			if (distance <= radius || distance2 <= radius) {
				glm::vec2 force = position - mObstacles[index].position;
				force = normalizeVec2(force) * AVOID_MAX_FORCE;
				avoid += force;
				collision = true;
			}
		}

		if (collision) {
			mData[index].moveDirection =
				rotateVec2(glm::vec2(1.0f, 0.0f), random.nextFloat(0.0f, TWO_PI));
		}

		mData[index].avoidForce = avoid;

		//	Set forces
		physicsSystem.addForce(physicsCmpnt, avoid);

		//	Set movement direction
		physicsSystem.setDirection(physicsCmpnt, mData[index].moveDirection);
	}
}
}