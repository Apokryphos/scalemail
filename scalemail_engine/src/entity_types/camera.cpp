#include "ai_system.hpp"
#include "camera_system.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "world.hpp"
#include <string>

namespace ScaleMail
{
//  ============================================================================
Entity createCamera(World& world, const std::string& name) {
	Entity entity = world.createEntity();

	AiSystem& aiSystem = world.getAiSystem();
	aiSystem.addComponent(entity);
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setAvoid(aiCmpnt, false);

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setAcceleration(physicsCmpnt, 2.33f);
	physicsSystem.setRadius(physicsCmpnt, 0.0f);
	physicsSystem.setMaxSpeed(physicsCmpnt, 100.0f);

	//	Set name
	NameSystem& nameSystem = world.getNameSystem();
	nameSystem.addComponent(entity);
	NameComponent nameCmpnt = nameSystem.getComponent(entity);
	nameSystem.setName(nameCmpnt, name);

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.addComponent(entity);

	return entity;
}
}