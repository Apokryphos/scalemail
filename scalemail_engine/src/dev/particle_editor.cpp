#include "game.hpp"
#include "math_util.hpp"
#include "particle_system.hpp"
#include "world.hpp"
#include <imgui.h>

namespace ScaleMail
{
//  ============================================================================
static ParticleComponentData makeDefaultEmitter() {
	ParticleComponentData emitter = {};
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
	emitter.color = glm::vec4(1.0f);
	emitter.width = 4.0f;
	return emitter;
}

//  ============================================================================
void drawParticleEditor(Game& game, const Entity& entity) {
	World& world = *game.world;

	ImGui::Begin("Particle Editor");

	ParticleSystem& particleSystem = world.getParticleSystem();
	if (particleSystem.hasComponent(entity)) {
		ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

		ParticleComponentData data = particleSystem.getData(particleCmpnt);

		ImGui::SliderFloat("Life", &data.life, 0.0f, 100.0f);
		ImGui::SliderFloat("Decay", &data.decay, 0.0f, 100.0f);
		ImGui::SliderFloat("Duration", &data.duration, 0.0f, 100.0f);
		ImGui::SliderInt("Emit Count", &data.emitCount, 0, 100);
		ImGui::SliderFloat("Interval", &data.interval, 0.0f, 100.0f);

		//	Only allow editing of X and Y components
		ImGui::SliderFloat2(
			"Direction",
			&data.direction[0],
			-1.0f,
			1.0f);

		ImGui::DragFloatRange2(
			"Size",
			&data.minSize,
			&data.maxSize,
			0.25f,
			0.0f,
			100.0f,
			"Min: %.1f",
			"Max: %.1f");

		ImGui::DragFloatRange2(
			"Speed",
			&data.minSpeed,
			&data.maxSpeed,
			0.25f,
			0.0f,
			100.0f,
			"Min: %.1f",
			"Max: %.1f");

		ImGui::SliderFloat("Spread", &data.spread, 0.0f, TWO_PI);
		ImGui::SliderFloat("Width", &data.width, 0.0f, 100.0f);
		ImGui::ColorEdit4("Color", &data.color[0]);

		particleSystem.setData(particleCmpnt, data);
	} else {
		if (ImGui::Button("Add Component")) {
			particleSystem.addComponent(entity);
			ParticleComponent particleCmpnt = particleSystem.getComponent(entity);
			particleSystem.setData(particleCmpnt, makeDefaultEmitter());
		}
	}

	ImGui::End();
}
}
