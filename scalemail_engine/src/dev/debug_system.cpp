#include "dev/debug_system.hpp"
#include "physics_system.hpp"
#include "vector_util.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/vec4.hpp>

namespace ScaleMail
{
//	============================================================================
static DebugComponent makeComponent(const int index) {
	return DebugComponent(index);
}

//	============================================================================
DebugSystem::DebugSystem(World& world, EntityManager& entityManager,
						 int maxComponents)
	: EntitySystem(entityManager, maxComponents), mWorld(world) {
	mData.reserve(maxComponents);
}

//	============================================================================
void DebugSystem::clearSelected() {
	const size_t count = mData.size();
	for (size_t c = 0; c < count; ++c) {
		mData[c].selected = false;
	}
}

//	============================================================================
void DebugSystem::createComponent() {
	const size_t cmpntIndex = mData.size();
	const Entity& entity = this->getEntityByComponentIndex(cmpntIndex);

	DebugComponentData data = {};
	data.entity = entity;
	data.idString = std::to_string(entity.id);

	mData.push_back(data);
}

//	============================================================================
void DebugSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//  ============================================================================
void DebugSystem::drawDebug(std::vector<float>& lineVertexData) {
	const int lineCount = 16;
	const glm::vec4 color = glm::vec4(0.25f, 0.75f, 0.25f, 1.0f);

	const PhysicsSystem& physicsSystem = mWorld.getPhysicsSystem();

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t cmpntIndex = p.first;

		const DebugComponentData& data = mData[cmpntIndex];

		if (data.selected) {
			const Entity& entity = p.second;

			if (!physicsSystem.hasComponent(entity)) {
				continue;
			}

			const PhysicsComponent physicsCmpnt =
				physicsSystem.getComponent(entity);

			const glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

			float radius = physicsSystem.getRadius(physicsCmpnt) * 2.0f;
			if (radius <= 0.0f) {
				radius = 12.0f;
			}

			addCircleVertexData(
				lineVertexData,
				lineCount,
				position,
				radius,
				color);
		}
	}
}

//	============================================================================
DebugComponent DebugSystem::getComponent(const Entity& entity) {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
const std::vector<DebugComponentData>& DebugSystem::getComponentData() const {
	return mData;
}

//	============================================================================
const std::string& DebugSystem::getIdString(const DebugComponent& cmpnt) const {
	return mData[cmpnt.index].idString;
}

//	============================================================================
void DebugSystem::setSelected(const DebugComponent& cmpnt, bool selected) {
	mData[cmpnt.index].selected = selected;
}
}