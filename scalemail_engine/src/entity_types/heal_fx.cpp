#include "entity.hpp"
#include "entity_util.hpp"
#include "expire_system.hpp"
#include "light_system.hpp"
#include "math_util.hpp"
#include "name_system.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "world.hpp"

namespace ScaleMail
{
//  ============================================================================
Entity createHealFx(World& world, const glm::vec2& position) {
	Entity entity = world.createEntity();

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, position);
	physicsSystem.setCollisionGroup(physicsCmpnt, CollisionGroup::NONE);
	physicsSystem.setRadius(physicsCmpnt, 0.0f);

	// const glm::vec2 size(24.0f);

	glm::vec4 lightColor(0.8706f, 0.1373f, 0.1373f, 1.0f);
	// const float lightSize = size.x * 1.5f;
	// const float lightGlowSize = size.x * 0.5f;
	// const float lightPulse = 16;
	// const float lightPulseSize = 3;

	const float duration = 0.3f;

	//	TODO: Add light component with decay effect, otherwise
	//	light just disappears after entity expires.

	// LightSystem& lightSystem = world.getLightSystem();
	// lightSystem.addComponent(entity);
	// LightComponent lightCmpnt = lightSystem.getComponent(entity);
	// lightSystem.setOffset(lightCmpnt, glm::vec2(0.0f, 0.0f));
	// lightSystem.setColor(lightCmpnt, lightColor);
	// lightSystem.setGlowSize(lightCmpnt, glm::vec2(lightGlowSize));
	// lightSystem.setSize(lightCmpnt, glm::vec2(lightSize));
	// lightSystem.setPulse(lightCmpnt, lightPulse);
	// lightSystem.setPulseSize(lightCmpnt, lightPulseSize);

	ParticleSystem& particleSystem = world.getParticleSystem();
	particleSystem.addComponent(entity);
	ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

	lightColor.a = 0.5f;

	ParticleComponentData emitter = {};
	emitter.life = 2.9f;
	emitter.decay = 1.0f;
	emitter.duration = duration;
	emitter.emitCount = 4;
	emitter.delay = 0.0f;
	emitter.interval = 0.1f;
	emitter.minSize = 0.25f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = 10.0f;
	emitter.maxSpeed = 16.0f;
	emitter.spread = toRadians(45.0f);
	emitter.direction = glm::vec3(0.0f, -1.5f, 2.0f);
	emitter.color = lightColor;
	emitter.height = 24.0f;
	emitter.width = 8.0f;
	emitter.radius = 8.0f;

	particleSystem.setData(particleCmpnt, emitter);

	ExpireSystem& expireSystem = world.getExpireSystem();
	expireSystem.addComponent(entity);
	ExpireComponent expireCmpnt = expireSystem.getComponent(entity);
	expireSystem.setDuration(expireCmpnt, duration);

	return entity;
}
}