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
	if (width == height) {
		const float radius = width * 0.5f;

		this->addObstacle(glm::vec2(x + radius, y + radius), radius);
	} else if (width > height) {
		const float radius = height * 0.5f;

		//	Add a circle to the right to cover the remainder
		this->addObstacle(
			glm::vec2(x + width - radius, y + radius),
			radius);

		int count = std::floor(width / height);
		for (float n = 0; n < count; ++n) {
			this->addObstacle(
				glm::vec2(x + radius + n * height, y + radius),
				radius);
		}

	} else {
		const float radius = width * 0.5f;

		//	Add a circle to the bottom to cover the remainder
		this->addObstacle(
			glm::vec2(x + radius, y + height - radius),
			radius);

		int count = std::floor(height / width);
		for (int n = 0; n < count; ++n) {
			this->addObstacle(
				glm::vec2(x + radius, y + radius + n * width),
				radius);
		}
	}
}

//	============================================================================
void AiSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void AiSystem::drawDebug(const Camera& camera) {
	const int lineCount = 16;
	const glm::vec4 circleColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	mLineVertexData.resize(0);

	for (const auto& obstacle : mObstacles) {
		addCircleVertexData(
			mLineVertexData,
			lineCount,
			obstacle.position,
			obstacle.radius,
			circleColor);
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

	//	Apply movements
	for (auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		physicsSystem.setDirection(physicsCmpnt, mData[index].moveDirection);

		mData[index].moveDirection = glm::vec2(0.0f);
	}
}
}