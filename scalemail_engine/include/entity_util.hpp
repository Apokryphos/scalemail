#pragma once

#include <string>

namespace ScaleMail
{
class AiBehaviorFactory;
class AiSystem;
struct BuryComponent;
class BurySystem;
struct Entity;
struct HealthComponent;
class HealthSystem;
class PrefabFactory;
class NameSystem;
class World;

//	Entity utility functions
//	These functions check entity components only and don't contain any game
//	rules logic.

void addAiBehavior(const std::string& ai, const Entity& entity,
				   AiSystem& aiSystem, AiBehaviorFactory& aiBehaviorFactory);

void addPrefab(const std::string& prefab, const Entity& entity, World& world);

void buryEntity(const Entity& entity, BurySystem& burySystem, bool immediate,
				float duration, bool spawnDirt);

bool entityIsAlive(const HealthSystem& healthSystem,
				   const HealthComponent& healthCmpnt);
bool entityIsAlive(const Entity& entity, const HealthSystem& healthSystem);

bool entityIsBuried(const BurySystem& burySystem,
					const BuryComponent& buryCmpnt);
bool entityIsBuried(const Entity& entity, const BurySystem& burySystem);

void setEntityName(const std::string name, const Entity& entity,
				   NameSystem& nameSystem);
}
