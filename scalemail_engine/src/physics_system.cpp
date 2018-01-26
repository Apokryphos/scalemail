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
	mPosition.reserve(maxComponents);
}

//	============================================================================
void PhysicsSystem::createComponent(const Entity& entity) {
	mPosition.emplace_back(0.0f);
}

//	============================================================================
void PhysicsSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mPosition, index);
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
void PhysicsSystem::setPosition(const PhysicsComponent& cmpnt,
								const glm::vec2 position) {
	mPosition[cmpnt.index] = position;
}


//	============================================================================
void PhysicsSystem::simulate(float elapsedSeconds) {
}
}