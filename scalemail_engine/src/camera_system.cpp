#include "camera_system.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
//	============================================================================
static CameraComponent makeComponent(const int index) {
	return CameraComponent(index);
}

//  ============================================================================
CameraSystem::CameraSystem(EntityManager& entityManager, int maxComponents)
: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//  ============================================================================
void CameraSystem::createComponent() {
	CameraComponentData data = {};
	mData.emplace_back(data);
}

//  ============================================================================
void CameraSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//  ============================================================================
CameraComponent CameraSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//  ============================================================================
const Camera& CameraSystem::getCamera(const CameraComponent& cmpnt) const {
	return mData[cmpnt.index].camera;
}

//  ============================================================================
void CameraSystem::update(PhysicsSystem& physicsSystem, float elapsedSeconds) {
	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		CameraComponentData& data = mData[index];

		if (!physicsSystem.hasComponent(data.targetEntity)) {
			continue;
		}

		const PhysicsComponent targetPhysicsCmpnt =
			physicsSystem.getComponent(data.targetEntity);

		const glm::vec2 position =
			physicsSystem.getPosition(targetPhysicsCmpnt);

		const PhysicsComponent cameraPhysicsCmpnt =
			physicsSystem.getComponent(entity);

		physicsSystem.setPosition(cameraPhysicsCmpnt, position);
	}
}
}