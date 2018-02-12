#include "asset_manager.hpp"
#include "light_system.hpp"
#include "physics_system.hpp"
#include "sprite_batch.hpp"
#include "vector_util.hpp"
#include <cmath>

namespace ScaleMail
{
//	============================================================================
static LightComponent makeComponent(const int index) {
	return LightComponent(index);
}

//	============================================================================
LightSystem::LightSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mOffset.reserve(maxComponents);
	mPosition.reserve(maxComponents);
	mColor.reserve(maxComponents);
	mGlowSize.reserve(maxComponents);
	mSize.reserve(maxComponents);
	mFinalSize.reserve(maxComponents);
	mFinalGlowSize.reserve(maxComponents);
	mPulse.reserve(maxComponents);
	mPulseSize.reserve(maxComponents);
	mTicks.reserve(0);
}

//	============================================================================
void LightSystem::buildGlowVertexData(SpriteBatch& spriteBatch) {
	spriteBatch.buildQuadVertexData(mLightTexture.id, mPosition, mFinalGlowSize,
									mColor, false);
}

//	============================================================================
void LightSystem::buildVertexData(SpriteBatch& spriteBatch) {
	spriteBatch.buildQuadVertexData(mLightTexture.id, mPosition, mFinalSize,
									mColor, false);
}

//	============================================================================
void LightSystem::createComponent() {
	mOffset.emplace_back(0.0f);
	mPosition.emplace_back(0.0f);
	mColor.emplace_back(1.0f);
	mGlowSize.emplace_back(16.0f);
	mFinalSize.emplace_back(0.0f);
	mFinalGlowSize.emplace_back(0.0f);
	mSize.emplace_back(32.0f);
	mPulse.emplace_back(0.0f);
	mPulseSize.emplace_back(0.0f);
	mTicks.emplace_back(0.0f);
}

//	============================================================================
void LightSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mOffset, index);
	swapWithLastElementAndRemove(mPosition, index);
	swapWithLastElementAndRemove(mColor, index);
	swapWithLastElementAndRemove(mGlowSize, index);
	swapWithLastElementAndRemove(mSize, index);
	swapWithLastElementAndRemove(mFinalGlowSize, index);
	swapWithLastElementAndRemove(mFinalSize, index);
	swapWithLastElementAndRemove(mPulse, index);
	swapWithLastElementAndRemove(mPulseSize, index);
	swapWithLastElementAndRemove(mTicks, index);
}

//	============================================================================
glm::vec4 LightSystem::getColor(const LightComponent& cmpnt) {
	return mColor[cmpnt.index];
}

//	============================================================================
LightComponent LightSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void LightSystem::initialize(AssetManager& assetManager) {
	mLightTexture = assetManager.loadTexture("light");
}

//	============================================================================
void LightSystem::setColor(const LightComponent& cmpnt, const glm::vec4 color) {
	mColor[cmpnt.index] = color;
}

//	============================================================================
void LightSystem::setGlowSize(const LightComponent& cmpnt, const float glowSize) {
	mGlowSize[cmpnt.index] = glm::vec2(glowSize);
}

//	============================================================================
void LightSystem::setOffset(const LightComponent& cmpnt, const glm::vec2 offset) {
	mOffset[cmpnt.index] = offset;
}

//	============================================================================
void LightSystem::setPulse(const LightComponent& cmpnt, const float pulse) {
	mPulse[cmpnt.index] = pulse;
}

//	============================================================================
void LightSystem::setPulseSize(const LightComponent& cmpnt, const float pulseSize) {
	mPulseSize[cmpnt.index] = pulseSize;
}

//	============================================================================
void LightSystem::setSize(const LightComponent& cmpnt, const float size) {
	mSize[cmpnt.index] = glm::vec2(size);
}

//	============================================================================
void LightSystem::update(float elapsedSeconds,
						   const PhysicsSystem& physicsSystem) {
	//	Update light positions from physics system
	glm::vec2 position;
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		position =
			physicsSystem.getPosition(physicsSystem.getComponent(p.second));

		const float lightLayer = -0.9f;
		mPosition[index] = glm::vec3(position + mOffset[index], lightLayer);
	}

	//  Update animated lights
	for (auto& p : mEntitiesByComponentIndices) {
		const int index = p.first;

		mTicks[index] += elapsedSeconds * 0.25f;

		const float size = mPulseSize[index] * sin(mTicks[index] * mPulse[index]);

		mFinalGlowSize[index] = size * 0.25f + mGlowSize[index];
		mFinalSize[index] = size + mSize[index];
	}
}
}