#include "camera_system.hpp"
#include "map.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
static CameraComponent makeComponent(const int index) {
	return CameraComponent(index);
}

//  ============================================================================
CameraSystem::CameraSystem(World& world, EntityManager& entityManager,
						   int maxComponents)
: EntitySystem(entityManager, maxComponents), mWorld(world) {
	mData.reserve(maxComponents);
}

//  ============================================================================
void CameraSystem::createComponent() {
	CameraComponentData data = {};
	data.mode = CameraMode::FIXED;

	data.camera.setSize(mWidth, mHeight);
	data.camera.setZoom(mZoom);

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
Camera& CameraSystem::getCamera(const CameraComponent& cmpnt) {
	return mData[cmpnt.index].camera;
}

//  ============================================================================
const Camera& CameraSystem::getCamera(const CameraComponent& cmpnt) const {
	return mData[cmpnt.index].camera;
}

//  ============================================================================
void CameraSystem::followEntity(const CameraComponent& cmpnt,
								const Entity& targetEntity) {
	mData[cmpnt.index].targetEntity = targetEntity;
	this->setMode(cmpnt, CameraMode::FOLLOW_ENTITY);
}

//  ============================================================================
void CameraSystem::initializeFollowEntity(const CameraComponent& cmpnt) {
	Entity cameraEntity = this->getEntityByComponentIndex(cmpnt.index);

	CameraComponentData& data = mData[cmpnt.index];

	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	if (!physicsSystem.hasComponent(data.targetEntity)) {
		return;
	}

	const PhysicsComponent targetPhysicsCmpnt =
		physicsSystem.getComponent(data.targetEntity);

	const glm::vec2 position =
		physicsSystem.getPosition(targetPhysicsCmpnt);

	const PhysicsComponent cameraPhysicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	this->updateBounds(position, data);

	physicsSystem.setPosition(cameraPhysicsCmpnt, position);

	data.camera.setPosition(position);
}

//  ============================================================================
void CameraSystem::initializeFollowPath(const CameraComponent& cmpnt) {
	CameraComponentData& data = mData[cmpnt.index];

	if (data.paths.size() == 0) {
		return;
	}

	if (data.paths[0].points.size() < 2) {
		return;
	}

	Entity cameraEntity = this->getEntityByComponentIndex(cmpnt.index);

	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	const glm::vec2 start = data.paths[0].points[0];

	const PhysicsComponent cameraPhysicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	this->updateBounds(start, data);

	data.camera.setPosition(start);
}

//  ============================================================================
void CameraSystem::setBounds(const CameraComponent& cmpnt,
							 const std::vector<Rectangle>& bounds) {
	mData[cmpnt.index].bounds = bounds;
}

//  ============================================================================
void CameraSystem::setCameraDefaults(const int width,
									 const int height,
									 const float zoom) {
	mWidth = width;
	mHeight = height;
	mZoom = zoom;
}

//  ============================================================================
void CameraSystem::setMode(const CameraComponent& cmpnt,
						   const CameraMode mode) {
	mData[cmpnt.index].mode = mode;

	switch (mode) {
		case CameraMode::FIXED:
			break;

		case CameraMode::FOLLOW_ENTITY:
			this->initializeFollowEntity(cmpnt);
			break;

		case CameraMode::FOLLOW_PATH:
			this->initializeFollowPath(cmpnt);
			break;
	}
}

//  ============================================================================
void CameraSystem::setPaths(const CameraComponent& cmpnt,
							const std::vector<Path>& paths) {
	mData[cmpnt.index].paths = paths;
}

//  ============================================================================
void CameraSystem::update(float elapsedSeconds) {
	World& world = mWorld;

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		CameraComponentData& data = mData[index];

		switch (data.mode) {
			case CameraMode::FIXED:
				this->updateFixed(world, entity, data);
				break;

			case CameraMode::FOLLOW_ENTITY:
				this->updateFollowEntity(world, entity, data);
				break;

			case CameraMode::FOLLOW_PATH:
				this->updateFollowPath(world, entity, data);
				break;
		}
	}
}

//  ============================================================================
void CameraSystem::updateBounds(const glm::vec2& position,
								CameraComponentData& data) {
	const std::vector<Rectangle>& bounds = data.bounds;

	if (bounds.size() == 0) {
		return;
	}

	//	Find map camera bounds rectangle the camera is currently positioned in
	auto findBounds = std::find_if(bounds.begin(), bounds.end(),
		[position](const Rectangle& b) -> bool {
			return b.contains(position);
		}
	);

	if (findBounds != bounds.end()) {
		//	Restrict camera to current map camera bounds rectangle
		data.camera.setBounds(*findBounds);
	}
}

//  ============================================================================
void CameraSystem::updateFixed(World& world, const Entity& cameraEntity,
							   CameraComponentData& data) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	const PhysicsComponent cameraPhysicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position = physicsSystem.getPosition(cameraPhysicsCmpnt);

	this->updateBounds(position, data);
}

//  ============================================================================
void CameraSystem::updateFollowEntity(World& world, const Entity& cameraEntity,
									  CameraComponentData& data) {
	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	if (!physicsSystem.hasComponent(data.targetEntity)) {
		return;
	}

	const PhysicsComponent targetPhysicsCmpnt =
		physicsSystem.getComponent(data.targetEntity);

	const glm::vec2 targetPosition =
		physicsSystem.getPosition(targetPhysicsCmpnt);

	const PhysicsComponent cameraPhysicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position =
		physicsSystem.getPosition(cameraPhysicsCmpnt);

	this->updateBounds(position, data);

	const glm::vec2 direction = normalize(targetPosition - position);

	physicsSystem.setDirection(cameraPhysicsCmpnt, direction);

	data.camera.setPosition(position);
}

//  ============================================================================
void CameraSystem::updateFollowPath(World& world, const Entity& cameraEntity,
									CameraComponentData& data) {
	if (data.paths.size() == 0) {
		return;
	}

	if (data.paths[0].points.size() < 2) {
		return;
	}

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();

	const PhysicsComponent cameraPhysicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position =
		physicsSystem.getPosition(cameraPhysicsCmpnt);

	const glm::vec2 end = data.paths[0].points[1];

	const glm::vec2 direction = normalize(end - position);

	physicsSystem.setDirection(cameraPhysicsCmpnt, direction);

	this->updateBounds(position, data);

	data.camera.setPosition(position);
}
}