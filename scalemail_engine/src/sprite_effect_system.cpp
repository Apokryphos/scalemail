#include "ease.hpp"
#include "sprite_effect_system.hpp"
#include "sprite_system.hpp"
#include "vector_util.hpp"
#include <cmath>

namespace ScaleMail
{
static const glm::vec4 WHITE_COLOR(1.0f);

//	============================================================================
static SpriteEffectComponent makeComponent(const int index) {
	return SpriteEffectComponent(index);
}

//	============================================================================
SpriteEffectSystem::SpriteEffectSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void SpriteEffectSystem::createComponent() {
	SpriteEffectComponentData data = {};
	data.blinkCount = 4.0f;
	data.blinkColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	data.blinkDuration = 0.0f;
	data.blinkTicks = 0.0f;

	mData.emplace_back(data);
}

//	============================================================================
void SpriteEffectSystem::bob(const SpriteEffectComponent& cmpnt, float speed) {
	mData[cmpnt.index].bobSpeed = speed;
}

//	============================================================================
void SpriteEffectSystem::buildVertexData(
	SpriteBatch& spriteBatch,
	SpriteSystem& spriteSystem) {
	std::vector<Entity> entities;

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;

		if (mData[index].blinkTicks > 0.0f) {
			const Entity& entity = p.second;
			entities.push_back(entity);
		}
	}

	spriteSystem.buildVertexData(spriteBatch, entities);
}

//	============================================================================
void SpriteEffectSystem::blink(const SpriteEffectComponent& cmpnt) {
	mData[cmpnt.index].blinkTicks = mData[cmpnt.index].blinkDuration;
}

//	============================================================================
void SpriteEffectSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
SpriteEffectComponent SpriteEffectSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void SpriteEffectSystem::setBlinkDuration(const SpriteEffectComponent& cmpnt,
										  const float duration) {
	mData[cmpnt.index].blinkDuration = duration;
}

//	============================================================================
void SpriteEffectSystem::update(float elapsedSeconds, SpriteSystem& spriteSystem) {
	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;

		mData[index].blinkTicks = std::max(
			mData[index].blinkTicks - elapsedSeconds,
			0.0f);

		mData[index].bobTicks += mData[index].bobSpeed * elapsedSeconds;
	}

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		SpriteComponent spriteCmpnt = spriteSystem.getComponent(entity);

		float d = mData[index].blinkDuration / mData[index].blinkCount;

		float t =
			fmod(mData[index].blinkTicks, d);

		glm::vec4 color = easeVec4(
			t,
			WHITE_COLOR,
			mData[index].blinkColor,
			d,
			easeInOutCubic);

		spriteSystem.setMaskColor(spriteCmpnt, color);

		if (mData[index].bobSpeed > 0.0f) {
			float offset = std::sin(mData[index].bobTicks) - 0.5f;
			spriteSystem.setOffsetY(spriteCmpnt, offset);
		}
	}
}
}