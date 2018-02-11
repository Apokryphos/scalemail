#include "bullet_system.hpp"
#include "vector_util.hpp"
#include "world.hpp"
#include <cmath>
#include <iostream>

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
	mSourceEntity.reserve(maxComponents);
}

//	============================================================================
void BulletSystem::createComponent() {
	mLife.emplace_back(1.5f);
	mSourceEntity.emplace_back();
}

//	============================================================================
void BulletSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mLife, index);
	swapWithLastElementAndRemove(mSourceEntity, index);
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
Entity BulletSystem::getSourceEntity(const BulletComponent& cmpnt) const {
	return mSourceEntity[cmpnt.index];
}

//	============================================================================
void BulletSystem::onEntityCollision(EntityCollision& collision) {
	if (collision.sourceGroup != CollisionGroup::PLAYER_BULLET &&
		collision.sourceGroup != CollisionGroup::BULLET) {
		return;
	}

	if (collision.targetGroup == CollisionGroup::PLAYER_BULLET ||
		collision.targetGroup == CollisionGroup::BULLET) {
		collision.ignore = true;
		return;
	}

	if (collision.sourceGroup == CollisionGroup::PLAYER_BULLET &&
		collision.targetGroup == CollisionGroup::PLAYER_ACTOR) {
		collision.ignore = true;
		return;
	}

	if (collision.sourceGroup == CollisionGroup::BULLET &&
		collision.targetGroup == CollisionGroup::ACTOR) {
		collision.ignore = true;
		return;
	}

	if (this->hasComponent(collision.sourceEntity)) {
		BulletComponent cmpnt = this->getComponent(collision.sourceEntity);

		if (mSourceEntity[cmpnt.index] != collision.targetEntity) {
			mLife[cmpnt.index] = 0;
		} else {
			//	Ignore source entity (i.e. entity that fired bullet)
			collision.ignore = true;
		}
	}
}

//	============================================================================
void BulletSystem::onStaticCollision(StaticCollision& collision) {
	if (collision.sourceGroup != CollisionGroup::PLAYER_BULLET &&
		collision.sourceGroup != CollisionGroup::BULLET) {
		return;
	}

	if (this->hasComponent(collision.sourceEntity)) {
		BulletComponent cmpnt = this->getComponent(collision.sourceEntity);
		mLife[cmpnt.index] = 0;
	}
}

//	============================================================================
void BulletSystem::setLife(const BulletComponent& cmpnt, float life) {
	mLife[cmpnt.index] = life;
}

//	============================================================================
void BulletSystem::setSourceEntity(const BulletComponent& cmpnt, Entity entity) {
	mSourceEntity[cmpnt.index] = entity;
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