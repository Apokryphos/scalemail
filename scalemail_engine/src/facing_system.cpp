#include "direction_util.hpp"
#include "entity_manager.hpp"
#include "facing_system.hpp"
#include "physics_system.hpp"
#include "sprite_system.hpp"

namespace ScaleMail
{
//	============================================================================
static FacingComponent makeComponent(const int index) {
	return FacingComponent(index);
}

//	============================================================================
FacingSystem::FacingSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
}

//	============================================================================
void FacingSystem::createComponent() {
}

//	============================================================================
void FacingSystem::destroyComponent([[maybe_unused]]int index) {
}

//	============================================================================
FacingComponent FacingSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void FacingSystem::update(PhysicsSystem& physicsSystem,
						  SpriteSystem& spriteSystem) {
	//	Update sprite facings from position system
	glm::vec2 direction;
	for (const auto& p : mEntitiesByComponentIndices) {
		const Entity& entity = p.second;

		direction =
			physicsSystem.getDirection(physicsSystem.getComponent(entity));

		if (direction.x != 0.0f || direction.y != 0.0f) {
			spriteSystem.setFacing(
				spriteSystem.getComponent(entity),
				vec2ToDirection(direction));
		}
	}
}
}