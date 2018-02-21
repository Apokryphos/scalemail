#include "ai_behavior.hpp"
#include "ai_system.hpp"
#include "entity.hpp"
#include "bury_system.hpp"
#include "health_system.hpp"
#include "name_system.hpp"
#include "prefab_factory.hpp"
#include "world.hpp"

namespace ScaleMail
{

//	============================================================================
void addAiBehavior(const std::string& ai, const Entity& entity,
				   AiSystem& aiSystem, AiBehaviorFactory& aiBehaviorFactory) {
	if (ai != "") {
		std::shared_ptr<AiBehavior> aiBehavior =
			aiBehaviorFactory.createAiBehavior(ai);

		if (aiBehavior != nullptr) {
			aiBehavior->setEntity(entity);

			if (!aiSystem.hasComponent(entity)) {
				aiSystem.addComponent(entity);
			}

			AiComponent aiCmpnt = aiSystem.getComponent(entity);
			aiSystem.addBehavior(aiCmpnt, aiBehavior);
		}
	}
}

//	============================================================================
void addPrefab(const std::string& prefab, const Entity& entity, World& world) {
	if (prefab != "") {
		world.getPrefabFactory().buildPrefab(entity, prefab, world);
	}
}

//	============================================================================
void buryEntity(const Entity& entity, BurySystem& burySystem, bool immediate,
				float duration, bool spawnDirt) {
	if (!burySystem.hasComponent(entity)) {
		burySystem.addComponent(entity);
	}

	BuryComponent buryCmpnt = burySystem.getComponent(entity);
	burySystem.setSpawnDirt(buryCmpnt, spawnDirt);
	burySystem.setDuration(buryCmpnt, duration);
	burySystem.bury(buryCmpnt, immediate);
}

//	============================================================================
bool entityIsAlive(const HealthSystem& healthSystem,
				   const HealthComponent& healthCmpnt) {
	return !healthSystem.getHealthGauge(healthCmpnt).isEmpty();
}

//	============================================================================
bool entityIsAlive(const Entity& entity, const HealthSystem& healthSystem) {
	if (healthSystem.hasComponent(entity)) {
		const HealthComponent healthCmpnt = healthSystem.getComponent(entity);
		return entityIsAlive(healthSystem, healthCmpnt);
	}

	return false;
}

//	============================================================================
bool entityIsBuried(const BurySystem& burySystem,
				   const BuryComponent& buryCmpnt) {
	return burySystem.getBuryState(buryCmpnt) != BuryState::NORMAL;
}

//	============================================================================
bool entityIsBuried(const Entity& entity, const BurySystem& burySystem) {
	if (burySystem.hasComponent(entity)) {
		const BuryComponent buryCmpnt = burySystem.getComponent(entity);
		return entityIsBuried(burySystem, buryCmpnt);
	}

	return false;
}

//	============================================================================
void setEntityName(const std::string name, const Entity& entity,
				   NameSystem& nameSystem) {
	if (name != "") {
		if (!nameSystem.hasComponent(entity)) {
			nameSystem.addComponent(entity);
		}

		NameComponent nameCmpnt = nameSystem.getComponent(entity);
		nameSystem.setName(nameCmpnt, name);
	}
}
}