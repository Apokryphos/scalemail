#include "entity.hpp"
#include "bury_system.hpp"
#include "health_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
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
}