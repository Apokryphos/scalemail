#include "entity.hpp"
#include "bury_system.hpp"
#include "health_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
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