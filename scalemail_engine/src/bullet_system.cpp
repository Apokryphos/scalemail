#include "bullet_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <cmath>

namespace ScaleMail
{
//	============================================================================
static BulletComponent makeComponent(const int index) {
	return BulletComponent(index);
}

//	============================================================================
BulletSystem::BulletSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mLife.reserve(maxComponents);
}

//	============================================================================
void BulletSystem::createComponent() {
	mLife.emplace_back(1.5f);
}

//	============================================================================
void BulletSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mLife, index);
}

//	============================================================================
BulletComponent BulletSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
float BulletSystem::getLife(const BulletComponent& cmpnt) const {
	return mLife[cmpnt.index];
}

//	============================================================================
void BulletSystem::onStaticCollision(StaticCollision collision) {
	if (collision.sourceGroup == CollisionGroup::BULLET &&
		this->hasComponent(collision.sourceEntity)) {
		BulletComponent cmpnt = this->getComponent(collision.sourceEntity);
		mLife[cmpnt.index] = 0;
	}
}

//	============================================================================
void BulletSystem::setLife(const BulletComponent& cmpnt, float life) {
	mLife[cmpnt.index] = life;
}

//	============================================================================
void BulletSystem::simulate(World& world, float elapsedSeconds) {
	std::vector <Entity> removeEntities;

	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		mLife[index] -= elapsedSeconds;

		if (mLife[index] <= 0.0f) {
			removeEntities.push_back(p.second);
		}
	}

	for (auto& entity : removeEntities) {
		world.destroyBullet(entity);
	}
}
}