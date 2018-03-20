#include "ai_system.hpp"
#include "camera_system.hpp"
#include "ease.hpp"
#include "map.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <glm/gtx/norm.hpp>

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
void CameraSystem::addBounds(const Rectangle& bounds, bool visited) {
	CameraVisibility visibility = {};
	visibility.alphaDuration = 0.66f;
	visibility.visited = visited;
	visibility.bounds = bounds;

	mVisibility.push_back(visibility);
}

//  ============================================================================
void CameraSystem::addPath(const std::string& name, const Path& path) {
	mPaths[name] = path;
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
const std::vector<CameraVisibility> CameraSystem::getVisibility() const {
	return mVisibility;
}

//  ============================================================================
void CameraSystem::followEntity(const CameraComponent& cmpnt,
								const Entity& targetEntity) {
	mData[cmpnt.index].targetEntity = targetEntity;
	this->setMode(cmpnt, CameraMode::FOLLOW_ENTITY);
}

//  ============================================================================
void CameraSystem::initializeFixed(const CameraComponent& cmpnt) {
	Entity cameraEntity = this->getEntityByComponentIndex(cmpnt.index);
	CameraComponentData& data = mData[cmpnt.index];

	this->updateFixed(cameraEntity, data);
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

	const PhysicsComponent physicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	this->updateBounds(position, data);

	data.camera.setPosition(position);

	physicsSystem.setPosition(physicsCmpnt, data.camera.getPosition());

	//	Enable acceleration
	physicsSystem.setAcceleration(physicsCmpnt, 2.0f);
}

//  ============================================================================
void CameraSystem::initializeFollowPath(const CameraComponent& cmpnt) {
	CameraComponentData& data = mData[cmpnt.index];

	if (data.path.points.size() < 2) {
		return;
	}

	Entity cameraEntity = this->getEntityByComponentIndex(cmpnt.index);

	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	//	Restrict path end camera position to bounds
	const glm::vec2 end = data.path.points[1];
	this->updateBounds(end, data);
	data.camera.setPosition(end);
	data.pathEnd = data.camera.getPosition();

	//	Restrict path start camera position to bounds
	const glm::vec2 start = data.path.points[0];
	this->updateBounds(start, data);
	data.camera.setPosition(start);
	data.pathStart = data.camera.getPosition();

	data.camera.setBounds(mWorld.getMap()->getBounds());

	const PhysicsComponent physicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	//	Set initial camera position
	physicsSystem.setPosition(physicsCmpnt, data.pathStart);

	//	Enable acceleration
	physicsSystem.setAcceleration(physicsCmpnt, 2.0f);
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
			this->initializeFixed(cmpnt);
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
void CameraSystem::setPath(const CameraComponent& cmpnt,
						   const std::string& pathName) {
	mData[cmpnt.index].path = mPaths[pathName];
}


//  ============================================================================
void CameraSystem::update(float elapsedSeconds) {
	for (auto& v : mVisibility) {
		v.alphaDirection = -1.0f;
	}

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		CameraComponentData& data = mData[index];

		switch (data.mode) {
			case CameraMode::FIXED:
				this->updateFixed(entity, data);
				break;

			case CameraMode::FOLLOW_ENTITY:
				this->updateFollowEntity(entity, data);
				break;

			case CameraMode::FOLLOW_PATH:
				this->updateFollowPath(entity, data);
				break;
		}

		this->updateVisibility(data.camera);
	}

	for (auto& v : mVisibility) {
		v.alphaTicks = clamp(
			v.alphaTicks + v.alphaDirection * elapsedSeconds,
			0.0f,
			v.alphaDuration);

		v.alpha = easeOutCubic(
			v.alphaTicks,
			0.0f,
			1.0f,
			v.alphaDuration);
	}
}

//  ============================================================================
void CameraSystem::updateBounds(const glm::vec2& position,
								CameraComponentData& data) {
	if (mVisibility.size() == 0) {
		return;
	}

	//	Find map camera bounds rectangle the camera is currently positioned in
	const auto& find = std::find_if(mVisibility.begin(), mVisibility.end(),
		[position](const auto& v) -> bool {
			return v.bounds.contains(position);
		}
	);

	if (find != mVisibility.end()) {
		data.camera.setBounds((*find).bounds);
	}
}

//  ============================================================================
void CameraSystem::updateFixed(const Entity& cameraEntity,
							   CameraComponentData& data) {
	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	const PhysicsComponent physicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

	data.camera.setBounds(mWorld.getMap()->getBounds());

	data.camera.setPosition(position);

	physicsSystem.setPosition(physicsCmpnt, position);
}

//  ============================================================================
void CameraSystem::updateFollowEntity(const Entity& cameraEntity,
									  CameraComponentData& data) {
	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	if (!physicsSystem.hasComponent(data.targetEntity)) {
		return;
	}

	//	Update camera bounds and position from last physics update
	const PhysicsComponent physicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position =
		physicsSystem.getPosition(physicsCmpnt);

	this->updateBounds(position, data);
	data.camera.setPosition(position);

	//	Seek target position
	const PhysicsComponent targetPhysicsCmpnt =
		physicsSystem.getComponent(data.targetEntity);

	const glm::vec2 targetPosition =
		physicsSystem.getPosition(targetPhysicsCmpnt);

	AiSystem& aiSystem = mWorld.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(cameraEntity);
	aiSystem.setSeek(aiCmpnt, true);
	aiSystem.setSeekTarget(aiCmpnt, targetPosition);
}

//  ============================================================================
void CameraSystem::updateFollowPath(const Entity& cameraEntity,
									CameraComponentData& data) {
	if (data.path.points.size() < 2) {
		return;
	}

	//	Update camera position from last physics update
	PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	const PhysicsComponent physicsCmpnt =
		physicsSystem.getComponent(cameraEntity);

	const glm::vec2 position =
		physicsSystem.getPosition(physicsCmpnt);

	data.camera.setBounds(mWorld.getMap()->getBounds());

	data.camera.setPosition(position);

	//	Seek path end position
	AiSystem& aiSystem = mWorld.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(cameraEntity);
	aiSystem.setSeek(aiCmpnt, true);
	aiSystem.setSeekTarget(aiCmpnt, data.pathEnd);
}

//  ============================================================================
void CameraSystem::updateVisibility(const Camera& camera) {
	if (mVisibility.size() == 0) {
		return;
	}

	Rectangle cameraRect = camera.getRectangle();

	//	Find map camera bounds rectangle the camera is overlapping
	for (auto& v : mVisibility) {
		if (v.visited) {
			if (v.bounds.intersects(cameraRect)) {
			//	Make bounds visible
				v.alphaDirection = 1.0f;
			}
		} else {
			v.visited = v.bounds.contains(camera.getPosition());
		}
	}
}
}