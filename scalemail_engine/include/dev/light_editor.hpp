#pragma once

namespace ScaleMail
{
struct Entity;
class Game;
struct LightData;
class LightSystem;

void drawLightDataEditor(LightData& data);
void drawLightComponentEditor(LightSystem& lightSystem, const Entity& entity);
void drawLightComponentEditorWindow(LightSystem& lightSystem, const Entity& entity);
}
