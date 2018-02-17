#include "ai_behavior.hpp"
#include "ai_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"

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
void AiSystem::addBehavior(const AiComponent& cmpnt,
						   std::shared_ptr<AiBehavior> behavior) {
	mData[cmpnt.index].behaviors.push_back(behavior);
}

//	============================================================================
void AiSystem::addObstacle(const glm::vec2& position, const float radius) {
	Obstacle obstacle = {};
	obstacle.position = position;
	obstacle.radius = radius;

	mObstacles.push_back(obstacle);
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
	const size_t count = mData.size();
	for (size_t index = 0; index < count; ++index) {
		for (auto& behavior : mData[index].behaviors) {
			behavior.get()->think(world, elapsedSeconds);
		}
	}
}
}