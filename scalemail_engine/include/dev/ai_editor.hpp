#pragma once

namespace ScaleMail
{
struct AiData;
struct AiDebugData;
class AiSystem;
struct Entity;

void drawAiDataEditor(AiData& aiData);
void drawAiDebugDataEditor(AiDebugData& aiDebugData);
void drawAiComponentEditor(AiSystem& aiSystem, const Entity& entity);
void drawAiComponentEditorWindow(AiSystem& aiSystem, const Entity& entity);
}
