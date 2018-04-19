#pragma once

namespace ScaleMail
{
struct Entity;
struct PhysicsData;
struct PhysicsDebugData;
class PhysicsSystem;

void drawPhysicsDataEditor(PhysicsData& data);
void drawPhysicsDebugDataEditor(PhysicsDebugData& data);
void drawPhysicsComponentEditor(PhysicsSystem& physicsSystem,
								const Entity& entity);
void drawPhysicsComponentEditorWindow(PhysicsSystem& physicsSystem,
									  const Entity& entity);
}
