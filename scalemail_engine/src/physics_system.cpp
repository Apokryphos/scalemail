#include "asset_manager.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "collision_test.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "vertex_data.hpp"
#include <cmath>
#include <functional>

namespace ScaleMail
{
//	============================================================================
static PhysicsComponent makeComponent(const int index) {
	return PhysicsComponent(index);
}

//	============================================================================
PhysicsSystem::PhysicsSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mGroup.reserve(maxComponents);
	mDirection.reserve(maxComponents);
	mPosition.reserve(maxComponents);
	mRadius.reserve(maxComponents);
	mSpeed.reserve(maxComponents);
	mVelocity.reserve(maxComponents);
}

//	============================================================================
void PhysicsSystem::addEntityCollisionCallback(
	std::function<void(EntityCollision&)> callback) {
	mEntityCollisionCallbacks.push_back(callback);
}

//	============================================================================
void PhysicsSystem::addStaticCollisionCallback(
	std::function<void(StaticCollision&)> callback) {
	mStaticCollisionCallbacks.push_back(callback);
}

//	============================================================================
void PhysicsSystem::addStaticActorObstacle(const float x, const float y,
									  	   const float width, const float height) {
	mStaticActorObstacles.emplace_back(x ,y ,width, height);
}

//	============================================================================
void PhysicsSystem::addStaticObstacle(const float x,     const float y,
									  const float width, const float height) {
	mStaticObstacles.emplace_back(x ,y ,width, height);
}

//	============================================================================
void PhysicsSystem::clearStaticObstacles() {
	mStaticObstacles.clear();
}

//	============================================================================
void PhysicsSystem::drawDebug(const Camera& camera) {
	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	const int lineCount = 16;

	mLineVertexData.resize(0);

	const glm::vec4 circleColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		glm::vec2 position = mPosition[index];

		float radius = mRadius[index];

		for (int n = 0; n < lineCount; ++n) {
			float x1 = position.x + radius * cos(n * TWO_PI / lineCount);
			float x2 = position.x + radius * cos((n + 1) * TWO_PI / lineCount);

			float y1 = position.y + radius * sin(n * TWO_PI / lineCount);
			float y2 = position.y + radius * sin((n + 1) * TWO_PI / lineCount);

			mLineVertexData.emplace_back(x1);
			mLineVertexData.emplace_back(y1);
			mLineVertexData.emplace_back(circleColor.r);
			mLineVertexData.emplace_back(circleColor.g);
			mLineVertexData.emplace_back(circleColor.b);
			mLineVertexData.emplace_back(circleColor.a);

			mLineVertexData.emplace_back(x2);
			mLineVertexData.emplace_back(y2);
			mLineVertexData.emplace_back(circleColor.r);
			mLineVertexData.emplace_back(circleColor.g);
			mLineVertexData.emplace_back(circleColor.b);
			mLineVertexData.emplace_back(circleColor.a);
		}
	}

	const glm::vec4 staticColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	for (const auto& rect : mStaticObstacles) {
		glm::vec2 position = glm::vec2(rect.x, rect.y);
		glm::vec2 size = glm::vec2(rect.z, rect.w);

		addQuadLineVertexData(mLineVertexData, position, size, staticColor);
	}

	const glm::vec4 staticActorColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

	for (const auto& rect : mStaticActorObstacles) {
		glm::vec2 position = glm::vec2(rect.x, rect.y);
		glm::vec2 size = glm::vec2(rect.z, rect.w);

		addQuadLineVertexData(mLineVertexData, position, size, staticActorColor);
	}

	updateMesh(mLineMesh, mLineVertexData);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(mLineShader.id);
	glUniformMatrix4fv(mLineShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindVertexArray(mLineMesh.vao);
	glDrawArrays(GL_LINES, 0, mLineMesh.vertexCount);
}

//	============================================================================
void PhysicsSystem::createComponent() {
	mGroup.emplace_back(CollisionGroup::NONE);
	mDirection.emplace_back(0.0f);
	mPosition.emplace_back(0.0f);
	mRadius.emplace_back(0.0f);
	mSpeed.emplace_back(64.0f);
	mVelocity.emplace_back(0.0f);
}

//	============================================================================
void PhysicsSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mGroup, index);
	swapWithLastElementAndRemove(mDirection, index);
	swapWithLastElementAndRemove(mPosition, index);
	swapWithLastElementAndRemove(mRadius, index);
	swapWithLastElementAndRemove(mSpeed, index);
	swapWithLastElementAndRemove(mVelocity, index);
}

//	============================================================================
PhysicsComponent PhysicsSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
CollisionGroup PhysicsSystem::getCollisionGroup(const PhysicsComponent& cmpnt) const {
	return mGroup[cmpnt.index];
}

//	============================================================================
glm::vec2 PhysicsSystem::getDirection(const PhysicsComponent& cmpnt) const {
	return mDirection[cmpnt.index];
}

//	============================================================================
std::vector<TriggerCollision> PhysicsSystem::getEntityIntersections(
	const std::vector<Trigger>& triggers) const {

	std::vector<TriggerCollision> collisions;

	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		if (mGroup[index] != CollisionGroup::ACTOR &&
			mGroup[index] != CollisionGroup::PLAYER_ACTOR) {
			continue;
		}

		const float radius = mRadius[index];
		if (radius <= 0) {
			continue;
		}

		const Entity& entity = p.second;
		const glm::vec2 position = mPosition[index];

		for (const auto& trigger : triggers) {
			if (circleIntersectsRectangle(position, radius, trigger.rect)) {
				TriggerCollision collision = {};
				collision.sourceEntity = entity;
				collision.triggerEntity = trigger.entity;
				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}

//	============================================================================
glm::vec2 PhysicsSystem::getPosition(const PhysicsComponent& cmpnt) const {
	return mPosition[cmpnt.index];
}

//	============================================================================
float PhysicsSystem::getRadius(const PhysicsComponent& cmpnt) const {
	return mRadius[cmpnt.index];
}

//	============================================================================
float PhysicsSystem::getSpeed(const PhysicsComponent& cmpnt) const {
	return mSpeed[cmpnt.index];
}

//	============================================================================
void PhysicsSystem::initialize(AssetManager& assetManager) {
	mLineShader = assetManager.getLineShader();
	initLineMesh(mLineMesh, {});
}

//	============================================================================
void PhysicsSystem::setCollisionGroup(const PhysicsComponent& cmpnt,
								 	   const CollisionGroup group) {
	mGroup[cmpnt.index] = group;
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
void PhysicsSystem::update() {
	//	Process entity collisions
	for (const auto& collision : mEntityCollisions) {
		const int index = this->getComponentIndexByEntity(collision.sourceEntity);
		mVelocity[index] = collision.velocity;
	}

	//	Process static collisions
	for (const auto& collision : mStaticCollisions) {
		const int index = this->getComponentIndexByEntity(collision.sourceEntity);
		mVelocity[index] = collision.velocity;
	}

	//	Apply velocities
	size_t count = mPosition.size();
	for (size_t index = 0; index < count; ++index) {
		mPosition[index] += mVelocity[index];
	}
}

//	============================================================================
void PhysicsSystem::simulate(float elapsedSeconds) {
	mEntityCollisions.resize(0);
	mStaticCollisions.resize(0);

	//	Calculate velocities
	size_t count = mPosition.size();
	for (size_t index = 0; index < count; ++index) {
		mVelocity[index] =
			mDirection[index] * mSpeed[index] * elapsedSeconds;
	}

	std::vector<CollisionTest> actorTests;
	std::vector<CollisionTest> bulletTests;
	std::vector<CollisionTest> playerActorTests;
	std::vector<CollisionTest> playerBulletTests;

	std::vector<CollisionTest> vsActorTests;
	std::vector<CollisionTest> vsBulletTests;
	std::vector<CollisionTest> vsPlayerActorTests;
	std::vector<CollisionTest> vsPlayerBulletTests;

	//	Separate entities by collision group
	for (const auto& p : mEntitiesByComponentIndices) {
		//	Skip zero radius entities
		if (mRadius[p.first] <= 0.0f) {
			continue;
		}

		CollisionTest test = {};
		test.entity = p.second;
		test.group = mGroup[p.first];
		test.position = mPosition[p.first];
		test.radius = mRadius[p.first];
		test.velocity = mVelocity[p.first];

		switch (mGroup[p.first]) {
			case CollisionGroup::ACTOR: {
				actorTests.push_back(test);

				vsActorTests.push_back(test);
				vsPlayerActorTests.push_back(test);
				vsPlayerBulletTests.push_back(test);
				break;
			}
			case CollisionGroup::PLAYER_ACTOR: {
				playerActorTests.push_back(test);

				vsActorTests.push_back(test);
				vsBulletTests.push_back(test);
				break;
			}
			case CollisionGroup::BULLET: {
				bulletTests.push_back(test);
				break;
			}
			case CollisionGroup::PLAYER_BULLET: {
				playerBulletTests.push_back(test);
				break;
			}
			case CollisionGroup::STATIC: {
				vsActorTests.push_back(test);
				vsPlayerActorTests.push_back(test);
				vsBulletTests.push_back(test);
				vsPlayerBulletTests.push_back(test);
				break;
			}
			default:
				break;
		}
	}

	std::vector<CollisionTest> actorPassed;

	//	--------------------
	//	TEST PLAYER ACTORS |
	//	--------------------
	//	VS static obstacles
	processStaticCollisions(playerActorTests, mStaticObstacles, actorPassed,
							mStaticCollisions);

	//	Clear and reuse for output of next pass
	playerActorTests.resize(0);

	//	VS static actor-only obstacles
	processStaticCollisions(actorPassed, mStaticActorObstacles, playerActorTests,
							mStaticCollisions);

	actorPassed.resize(0);

	//	VS entities
	processEntityCollisions(playerActorTests, vsPlayerActorTests, actorPassed,
							mEntityCollisions);

	//	-------------
	//	TEST ACTORS |
	//	-------------
	//	VS static obstacles
	processStaticCollisions(actorTests, mStaticObstacles, actorPassed,
							mStaticCollisions);

	actorTests.resize(0);

	//	VS static actor-only obstacles
	processStaticCollisions(actorPassed, mStaticActorObstacles, actorTests,
							mStaticCollisions);

	actorPassed.resize(0);

	//	VS entities
	processEntityCollisions(actorTests, vsActorTests, actorPassed, mEntityCollisions);

	//	--------------
	//	TEST BULLETS |
	//	--------------
	std::vector<CollisionTest> bulletPassed;

	//	VS static obstacles
	processStaticCollisions(bulletTests, mStaticObstacles, bulletPassed,
							mStaticCollisions);

	bulletTests.resize(0);

	//	Test bullets VS entities
	processEntityCollisions(bulletPassed, vsBulletTests, bulletTests, mEntityCollisions);

	//	---------------------
	//	TEST PLAYER BULLETS |
	//	---------------------
	bulletPassed.resize(0);
	//	Test player bullets VS static obstacles
	processStaticCollisions(playerBulletTests, mStaticObstacles, bulletPassed,
							mStaticCollisions);
	playerBulletTests.resize(0);
	//	Test player bullets VS entities
	processEntityCollisions(bulletPassed, vsPlayerBulletTests, playerBulletTests,
							mEntityCollisions);

	for (auto& collision : mEntityCollisions) {
		for (const auto& callback : mEntityCollisionCallbacks) {
			callback(collision);
		}
	}

	for (auto& collision : mStaticCollisions) {
		for (const auto& callback : mStaticCollisionCallbacks) {
			callback(collision);
		}
	}

	//	Remove ignored collisions
	mEntityCollisions.erase(
		std::remove_if(mEntityCollisions.begin(), mEntityCollisions.end(),
			[](const EntityCollision& collision) {
				 return collision.ignore;
			}),
		mEntityCollisions.end());

	mStaticCollisions.erase(
		std::remove_if(mStaticCollisions.begin(), mStaticCollisions.end(),
			[](const StaticCollision& collision) {
				 return collision.ignore;
			}),
		mStaticCollisions.end());
}
}