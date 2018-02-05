#include "asset_manager.hpp"
#include "camera.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include <cmath>

static const float TWO_PI = 6.28318530718f;

namespace ScaleMail
{
//	============================================================================
static PhysicsComponent makeComponent(const int index) {
	return PhysicsComponent(index);
}

//	============================================================================
PhysicsSystem::PhysicsSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mDirection.reserve(maxComponents);
	mPosition.reserve(maxComponents);
	mRadius.reserve(maxComponents);
	mSpeed.reserve(maxComponents);
}

//	============================================================================
void PhysicsSystem::drawDebug(const Camera& camera) {
	glm::mat4 mvp = camera.getProjection() * camera.getView();

	const int lineCount = 16;
	std::vector<float> vertexData;

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		glm::vec2 position = mPosition[index];
		float radius = mRadius[index];

		for (int n = 0; n < lineCount; ++n) {
			float x1 = position.x + radius * cos(n * TWO_PI / lineCount);
			float x2 = position.x + radius * cos((n + 1) * TWO_PI / lineCount);

			float y1 = position.y + radius * sin(n * TWO_PI / lineCount);
			float y2 = position.y + radius * sin((n + 1) * TWO_PI / lineCount);

			vertexData.push_back(x1);
			vertexData.push_back(y1);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);

			vertexData.push_back(x2);
			vertexData.push_back(y2);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);
			vertexData.push_back(1.0f);
		}
	}

	updateLineMesh(mLineMesh, vertexData);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(mLineShader.id);
	glUniformMatrix4fv(mLineShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(mLineMesh.vao);
	glDrawArrays(GL_LINES, 0, mLineMesh.vertexCount);
}

//	============================================================================
void PhysicsSystem::createComponent() {
	mDirection.emplace_back(0.0f);
	mPosition.emplace_back(0.0f);
	mRadius.emplace_back(8.0f);
	mSpeed.emplace_back(64.0f);
}

//	============================================================================
void PhysicsSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mDirection, index);
	swapWithLastElementAndRemove(mPosition, index);
	swapWithLastElementAndRemove(mRadius, index);
	swapWithLastElementAndRemove(mSpeed, index);
}

//	============================================================================
PhysicsComponent PhysicsSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
glm::vec2 PhysicsSystem::getPosition(const PhysicsComponent& cmpnt) const {
	return mPosition[cmpnt.index];
}

//	============================================================================
void PhysicsSystem::initialize(AssetManager& assetManager) {
	mLineShader = assetManager.getLineShader();
	initLineMesh(mLineMesh, {});
}

//	============================================================================
void PhysicsSystem::setDirection(const PhysicsComponent& cmpnt,
								const glm::vec2 direction) {
	mDirection[cmpnt.index] = direction;
}

//	============================================================================
void PhysicsSystem::setPosition(const PhysicsComponent& cmpnt,
								const glm::vec2 position) {
	mPosition[cmpnt.index] = position;
}

//	============================================================================
void PhysicsSystem::setRadius(const PhysicsComponent& cmpnt,
							  const float radius) {
	mRadius[cmpnt.index] = radius;
}

//	============================================================================
void PhysicsSystem::setSpeed(const PhysicsComponent& cmpnt,
							 const float speed) {
	mSpeed[cmpnt.index] = speed;
}

//	============================================================================
void PhysicsSystem::simulate(float elapsedSeconds) {
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		mPosition[index] += mDirection[index] * mSpeed[index] * elapsedSeconds;
	}
}
}