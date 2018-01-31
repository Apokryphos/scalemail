#include "physics_system.hpp"
#include "vector_util.hpp"

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
	mSpeed.reserve(maxComponents);
}

//	============================================================================
void PhysicsSystem::createComponent() {
	mDirection.emplace_back(0.0f);
	mPosition.emplace_back(0.0f);
	mSpeed.emplace_back(64.0f);
}

//	============================================================================
void PhysicsSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mDirection, index);
	swapWithLastElementAndRemove(mPosition, index);
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