#include "entity.hpp"
#include "entity_util.hpp"
#include "bury_system.hpp"
#include "health_system.hpp"
#include "physics_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//	============================================================================
bool actorCanFire(const Entity& entity, World& world) {
	//	Check if entity was recycled
	if (!world.entityIsAlive(entity)) {
		return false;
	}

	//	Must not be buried
	if (entityIsBuried(entity, world.getBurySystem())) {
		return false;
	}

	return true;
}

//	============================================================================
bool actorCanMove(const Entity& entity, World& world) {
	//	Check if entity was recycled
	if (!world.entityIsAlive(entity)) {
		return false;
	}

	//	Must have a PhysicsComponent
	if (!world.getPhysicsSystem().hasComponent(entity)) {
		return false;
	}

	//	Must have health
	if (!entityIsAlive(entity, world.getHealthSystem())) {
		return false;
	}

	//	Must not be buried
	if (entityIsBuried(entity, world.getBurySystem())) {
		return false;
	}

	return true;
}

//	============================================================================
bool actorIsAlive(const Entity& entity, World& world) {
	//	Check if entity was recycled
	if (!world.entityIsAlive(entity)) {
		return false;
	}

	//	Must have health
	if (!entityIsAlive(entity, world.getHealthSystem())) {
		return false;
	}

	return true;
}
}