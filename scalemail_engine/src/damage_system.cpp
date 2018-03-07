#include "damage_system.hpp"
#include "health_system.hpp"
#include "sprite_effect_system.hpp"
#include "vector_util.hpp"
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
static DamageComponent makeComponent(const int index) {
	return DamageComponent(index);
}

//	============================================================================
DamageSystem::DamageSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void DamageSystem::addDamage(const DamageComponent& cmpnt, const float damage) {
	mData[cmpnt.index].damage += damage;
}

//	============================================================================
void DamageSystem::addSourceEntity(const DamageComponent& cmpnt,
								   const Entity& source) {
	mData[cmpnt.index].sourceEntities.insert(source);
}

//	============================================================================
void DamageSystem::applyDamage(
	HealthSystem& healthSystem,
	SpriteEffectSystem& spriteEffectSystem) {
	for (const auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;
		const Entity& entity = p.second;

		float damage = mData[index].damage;

		if (damage > 0.0f && healthSystem.hasComponent(entity)) {
			HealthComponent healthCmpnt = healthSystem.getComponent(entity);
			HealthGauge& healthGauge = healthSystem.getHealthGauge(healthCmpnt);
			healthGauge.subtract(damage);
			mData[index].damage = 0.0f;

			//	Sprite damage blink effect
			if (spriteEffectSystem.hasComponent(entity)) {
				SpriteEffectComponent effectCmpnt =
					spriteEffectSystem.getComponent(entity);

				spriteEffectSystem.blink(effectCmpnt);
			}
		}
	}

	//	Remove expired source entities
	const size_t count = mData.size();
	for (size_t c = 0; c < count; ++c) {
		auto& sourceEntities = mData[c].sourceEntities;

		for(auto it = std::begin(sourceEntities); it != std::end(sourceEntities);) {
			if (!this->entityIsAlive(*it)) {
				it = sourceEntities.erase(it);
			} else {
				++it;
			}
		}
	}
}

//	============================================================================
void DamageSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void DamageSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
DamageComponent DamageSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
float DamageSystem::getDamage(const DamageComponent& cmpnt) const {
	return mData[cmpnt.index].damage;
}

//	============================================================================
const std::unordered_set<Entity>& DamageSystem::getSourceEntities(
	const DamageComponent& cmpnt) {
	return mData[cmpnt.index].sourceEntities;
}
}