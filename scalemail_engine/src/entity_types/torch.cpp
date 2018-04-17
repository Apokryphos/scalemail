#include "dev/debug_system.hpp"
#include "entity_types.hpp"
#include "light_system.hpp"
#include "particle_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
static const glm::vec4 torchLightColor(1.0f, 0.6f, 0.0f, 0.75f);
static const float torchLightSize = 64;
static const float torchLightGlowSize = torchLightSize * 0.25f;
static const float torchLightPulse = 8;
static const float torchLightPulseSize = 8;

//  ============================================================================
Entity createTorch(World& world, const glm::vec2& position,
				  const int frame1TilesetId, int frame2TilesetId) {
	Entity entity =
		createProp(world, position, frame1TilesetId, frame2TilesetId, false);

	world.getDebugSystem().addComponent(entity);

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);

	LightComponent lightCmpnt = lightSystem.getComponent(entity);

	lightSystem.setOffset(lightCmpnt, glm::vec2(0, 1));
	lightSystem.setColor(lightCmpnt, torchLightColor);
	lightSystem.setGlowSize(lightCmpnt, glm::vec2(torchLightGlowSize));
	lightSystem.setSize(lightCmpnt, glm::vec2(torchLightSize));
	lightSystem.setPulse(lightCmpnt, torchLightPulse);
	lightSystem.setPulseSize(lightCmpnt, torchLightPulseSize);

	ParticleSystem& particleSystem = world.getParticleSystem();
	particleSystem.addComponent(entity);

	ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

	ParticleEmitterData emitter = {};
	emitter.life = 1.6f;
	emitter.decay = 1.0f;
	emitter.duration = 1.0f;
	emitter.emitCount = 4;
	emitter.interval = 0.16f;
	emitter.minSize = 0.5f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = 4.0f;
	emitter.maxSpeed = 8.0f;
	emitter.spread = 0.349066f;
	emitter.direction = glm::vec3(0.0f, -1.0f, 2.0f);
	emitter.color = torchLightColor;
	emitter.width = 4.0f;

	particleSystem.setData(particleCmpnt, emitter);

	return entity;
}
}