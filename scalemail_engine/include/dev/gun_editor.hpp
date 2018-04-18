#pragma once

namespace ScaleMail
{
struct Entity;
class GunSystem;

void drawGunComponentEditor(GunSystem& gunSystem, const Entity& entity);
void drawGunComponentEditorWindow(GunSystem& gunSystem, const Entity& entity);
}
