#pragma once

#include "entity_system.hpp"
#include "texture.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
class AssetManager;
class PhysicsSystem;
class SpriteBatch;

struct LightComponent {
	LightComponent(const int index) { this->index = index; }
	int index;
};

class LightSystem : public EntitySystem
{
	Texture mLightTexture;

	std::vector<glm::vec2> mOffset;
	std::vector<glm::vec3> mPosition;
	std::vector<glm::vec4> mColor;
	std::vector<glm::vec2> mGlowSize;
	std::vector<glm::vec2> mSize;
	std::vector<glm::vec2> mFinalGlowSize;
	std::vector<glm::vec2> mFinalSize;
	std::vector<float> mPulse;
	std::vector<float> mPulseSize;
	std::vector<float> mTicks;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	LightSystem(EntityManager& entityManager, int maxComponents = 10000);
	void buildGlowVertexData(SpriteBatch& spriteBatch);
	void buildVertexData(SpriteBatch& spriteBatch);
	LightComponent getComponent(const Entity& entity) const;
	void initialize(AssetManager& assetManager);
	void setColor(const LightComponent& cmpnt, const glm::vec4 color);
	void setGlowSize(const LightComponent& cmpnt, const float glowSize);
	void setOffset(const LightComponent& cmpnt, const glm::vec2 offset);
	void setPulse(const LightComponent& cmpnt, const float pulse);
	void setPulseSize(const LightComponent& cmpnt, const float pulseSize);
	void setSize(const LightComponent& cmpnt, const float size);
	void update(float elapsedSeconds, const PhysicsSystem& physicsSystem);
};
}