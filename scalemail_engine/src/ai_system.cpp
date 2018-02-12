#include "ai_system.hpp"
#include "gun_system.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>

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
	// GunSystem& gunSystem = world.getGunSystem();
	// PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	// for (auto& p : mEntitiesByComponentIndices) {
		// const int index = p.first;
		// const Entity& entity = p.second;

		// AiComponentData& data = mData[index];

		// PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
		// glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);
		// physicsSystem.setDirection(physicsCmpnt, glm::vec2(1.0f, 0.0f));

		// GunComponent gunCmpnt = gunSystem.getComponent(entity);
		// gunSystem.setTarget(gunCmpnt, position + glm::vec2(0.0f, -1.0f));
		// gunSystem.setFire(gunCmpnt, true);
	// }
}
}