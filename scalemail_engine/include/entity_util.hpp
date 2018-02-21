#pragma once

namespace ScaleMail
{
struct BuryComponent;
class BurySystem;
struct Entity;
struct HealthComponent;
class HealthSystem;
class World;

//	Entity utility functions
//	These functions check entity components only and don't contain any game
//	rules logic.

void buryEntity(const Entity& entity, BurySystem& burySystem, bool immediate,
				float duration, bool spawnDirt);

bool entityIsAlive(const HealthSystem& healthSystem,
				   const HealthComponent& healthCmpnt);
bool entityIsAlive(const Entity& entity, const HealthSystem& healthSystem);

bool entityIsBuried(const BurySystem& burySystem,
					const BuryComponent& buryCmpnt);
bool entityIsBuried(const Entity& entity, const BurySystem& burySystem);
}
