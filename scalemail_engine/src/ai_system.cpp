#include "ai_behavior.hpp"
#include "ai_system.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "gl_headers.hpp"
#include "vector_util.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/mat4x4.hpp>

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

	//	Draw avoid force vector
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
	: EntitySystem(entityManager, maxComponents) {
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
void AiSystem::drawDebug(const Camera& camera) {
	const int lineCount = 16;
	const glm::vec4 obstacleColor = glm::vec4(0.25f, 0.75f, 1.0f, 1.0f);
	const glm::vec4 obstacleScaledColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	mLineVertexData.resize(0);

	for (const auto& obstacle : mObstacles) {
		addCircleVertexData(
			mLineVertexData,
			lineCount,
			obstacle.position,
			obstacle.radius,
			obstacleColor);

		addCircleVertexData(
			mLineVertexData,
			lineCount,
			obstacle.position,
			obstacle.radius * OBSTACLE_SCALE,
			obstacleScaledColor);
	}

	const glm::vec4 avoidColor(1.0f, 0.25f, 0.25f, 1.f);
	const glm::vec4 moveColor(0.25f, 0.25f, 1.0f, 1.0f);
	const glm::vec4 seekColor(0.25f, 1.0f, 0.25f, 1.0f);

	size_t c = 0;
	size_t v = 0;
	for (size_t n = 0; n < mData.size(); ++n) {
		const AiComponentData& data = mData[n];

		addLineVertexData(mLineVertexData, data.position, data.avoid, avoidColor);
		addLineVertexData(mLineVertexData, data.position, data.moveDirection, moveColor);
		addLineVertexData(mLineVertexData, data.position, data.seek, seekColor);
	}

	updateMesh(mLineMesh, mLineVertexData);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(mLineShader.id);
	glUniformMatrix4fv(mLineShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(mLineMesh.vao);
	glDrawArrays(GL_LINES, 0, mLineMesh.vertexCount);
}

//	============================================================================
void AiSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
AiComponent AiSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
void AiSystem::initialize(AssetManager& assetManager) {
	mLineShader = assetManager.getLineShader();
	initLineMesh(mLineMesh, {});
}

//	============================================================================
void AiSystem::setMoveDirection(const AiComponent& cmpnt, glm::vec2 direction) {
	mData[cmpnt.index].moveDirection = direction;
}

//	============================================================================
void AiSystem::update(World& world, float elapsedSeconds) {
	//	Update AI behaviors
	const size_t count = mData.size();
	for (size_t index = 0; index < count; ++index) {
		for (auto& behavior : mData[index].behaviors) {
			behavior.get()->think(world, elapsedSeconds);
		}
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	//	Amount to scale obstacle radius for purpose of avoidance
	const float MAX_AVOID_FORCE = 128.0f;

	for (auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

		//	Save position for debugging
		mData[index].position = position;

		//	Avoid obstacles
		glm::vec2 avoid(0.0f);
		const size_t obstacleCount = mObstacles.size();
		for (size_t index = 0; index < obstacleCount; ++index) {
			const float radius =
				(mObstacles[index].radius * OBSTACLE_SCALE) *
				(mObstacles[index].radius * OBSTACLE_SCALE);

			const float distance = glm::length2(position - mObstacles[index].position);

			if (distance < radius) {
				glm::vec2 force = position - mObstacles[index].position;
				force = normalizeVec2(force) * MAX_AVOID_FORCE;
				avoid += force;
			}
		}

		mData[index].avoid = avoid;

		//	Set forces
		physicsSystem.setForce(physicsCmpnt, avoid);

		//	Set movement directions
		physicsSystem.setDirection(physicsCmpnt, mData[index].moveDirection);
	}
}
}