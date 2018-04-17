#pragma once

#include "entity_system.hpp"
#include "mesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
struct AssetManager;
struct Entity;
class EntityManager;
class PhysicsSystem;
class Random;

struct ParticleComponent {
	ParticleComponent(const int index) { this->index = index; }
	int index;
};

//	Particle emitters
struct ParticleEmitterData {
	int emitCount;
	float life;
	float decay;
	float delay;
	float duration;
	float height;
	float interval;
	float minSize;
	float maxSize;
	float minSpeed;
	float maxSpeed;
	float radius;
	float spread;
	float ticks;
	float width;
	glm::vec3 direction;
	glm::vec4 color;
};

class ParticleSystem : public EntitySystem
{
private:
	//	Emitters
	std::vector<ParticleEmitterData> mData;

	//	Particles
	std::vector<float> mLife;
	std::vector<float> mLifetime;
	std::vector<float> mDecay;
	std::vector<float> mSize;
	std::vector<float> mSpeed;
	std::vector<float> mDirectionX;
	std::vector<float> mDirectionY;
	std::vector<float> mDirectionZ;
	std::vector<float> mPositionX;
	std::vector<float> mPositionY;
	std::vector<float> mPositionZ;
	std::vector<float> mColorR;
	std::vector<float> mColorG;
	std::vector<float> mColorB;
	std::vector<float> mColorA;

	std::vector<size_t> mRemoveParticles;

	Mesh mMesh;
	std::vector<float> mVertexData;

	Random* mRandom;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	ParticleSystem(EntityManager& entityManager, int maxComponents = 10000);
	void buildVertexData();
	ParticleComponent getComponent(const Entity& entity) const;
	const ParticleEmitterData& getData(const ParticleComponent& cmpnt) const;
	Mesh& getMesh();
	void initialize(const AssetManager& assetManager, Random& random);
	void setData(const ParticleComponent& cmpnt, const ParticleEmitterData& data);
	void update(PhysicsSystem& physicsSystem, float elapsedSeconds);
};
}