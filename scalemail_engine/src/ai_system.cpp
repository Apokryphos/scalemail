#include "ai_system.hpp"
#include "gun_system.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>
#include <iostream>

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
void AiSystem::createComponent() {
	mData.push_back({});
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
void AiSystem::update(World& world, float elapsedSeconds) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;
		const Entity& entity = p.second;

		AiComponentData& data = mData[index];

		PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		physicsSystem.setDirection(physicsCmpnt, glm::vec2(1.0f, 0.0f));
	}
}
}