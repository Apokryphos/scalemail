#pragma once

namespace ScaleMail
{
struct Entity;
struct ParticleEmitterData;
class ParticleSystem;

void drawParticleEmitterDataEditor(ParticleEmitterData& data);
void drawParticleComponentEditor(ParticleSystem& particleSystem,
								 const Entity& entity);
void drawParticleComponentEditorWindow(ParticleSystem& particleSystem,
									   const Entity& entity);
}
