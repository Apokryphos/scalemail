#include "door_system.hpp"
#include "name_system.hpp"
#include "physics_system.hpp"
#include "trigger.hpp"
#include "trigger_collision.hpp"
#include "trigger_system.hpp"
#include "vector_util.hpp"
#include <glm/vec4.hpp>
#include <iostream>

namespace ScaleMail
{
//	============================================================================
static TriggerComponent makeComponent(const int index) {
	return TriggerComponent(index);
}

//	============================================================================
TriggerSystem::TriggerSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void TriggerSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void TriggerSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
TriggerComponent TriggerSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
void TriggerSystem::setPosition(const TriggerComponent& cmpnt,
								const glm::vec2 position) {
	mData[cmpnt.index].position = position;
}

//	============================================================================
void TriggerSystem::setSize(const TriggerComponent& cmpnt,
							const glm::vec2 size) {
	mData[cmpnt.index].size = size;
}

//	============================================================================
void TriggerSystem::setTargetName(const TriggerComponent& cmpnt,
								  const std::string targetName) {
	mData[cmpnt.index].targetName = targetName;
}

//	============================================================================
void TriggerSystem::update(
	const NameSystem& nameSystem,
	const PhysicsSystem& physicsSystem,
	DoorSystem& doorSystem) {
	std::vector<Trigger> triggers;

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;
		const Entity& entity = p.second;

		TriggerComponentData& data = mData[index];

		data.lastEntities = data.entities;
		data.entities.clear();

		Trigger trigger = {};
		trigger.entity = entity;
		trigger.rect = glm::vec4(data.position.x, data.position.y,
					   data.size.x, data.size.y);

		triggers.push_back(trigger);
	}

	std::vector<TriggerCollision> collisions;
	collisions = physicsSystem.getEntityIntersections(triggers);

	for (auto& collision : collisions) {
		TriggerComponent cmpnt =
			this->getComponent(collision.triggerEntity);

		TriggerComponentData& data = mData[cmpnt.index];
		data.entities.push_back(collision.sourceEntity);
	}

	size_t count = mData.size();
	for (size_t index = 0; index < count; ++index) {
		TriggerComponentData& data = mData[index];

		if (data.entities.size() > 0) {
			std::vector<Entity> entities =
				nameSystem.getEntitiesByName(data.targetName);

			for (auto& entity : entities) {
				if (doorSystem.hasComponent(entity)) {
					const DoorComponent doorCmpnt = doorSystem.getComponent(entity);
					doorSystem.setOpen(doorCmpnt, true);
				}
			}
		}
	}
}
}