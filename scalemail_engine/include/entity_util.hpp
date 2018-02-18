#pragma once

namespace ScaleMail
{
struct BuryComponent;
class BurySystem;
struct HealthComponent;
class HealthSystem;
struct Entity;
class World;

//	Entity utility functions
//	These functions check entity components only and don't contain any game
//	rules logic.

bool entityIsAlive(const HealthSystem& healthSystem,
				   const HealthComponent& healthCmpnt);
bool entityIsAlive(const Entity& entity, const HealthSystem& healthSystem);

bool entityIsBuried(const BurySystem& burySystem,
					const BuryComponent& buryCmpnt);
bool entityIsBuried(const Entity& entity, const BurySystem& burySystem);
}
