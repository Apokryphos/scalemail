#include "ease.hpp"
#include "bury_system.hpp"
#include "entity_manager.hpp"
#include "math_util.hpp"
#include "physics_system.hpp"
#include "random.hpp"
#include "sprite_system.hpp"
#include "vector_util.hpp"
#include <cmath>

namespace ScaleMail
{
static const glm::vec4 WHITE_COLOR(1.0f);

//	============================================================================
static BuryComponent makeComponent(const int index) {
	return BuryComponent(index);
}

//	============================================================================
BurySystem::BurySystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void BurySystem::bury(const BuryComponent& cmpnt, bool immediate) {
	BuryComponentData& data = mData[cmpnt.index];

	const Entity& entity = this->getEntityByComponentIndex(cmpnt.index);
	SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);

	if (data.buryState == BuryState::NORMAL) {
		data.collisionRadius = mPhysicsSystem->getRadius(physicsCmpnt);
		data.offsetY = mSpriteSystem->getOffsetY(spriteCmpnt);
		data.size = mSpriteSystem->getSize(spriteCmpnt);
		data.sourceRect = mSpriteSystem->getSourceRect(spriteCmpnt);
	}

	if (immediate) {
		data.buryState = BuryState::BURIED;
		data.ticks = 0;
		mPhysicsSystem->setRadius(physicsCmpnt, 0.0f);
		mSpriteSystem->setSize(spriteCmpnt, glm::vec2(0.0f, 0.0f));
		mSpriteSystem->animate(spriteCmpnt, false);
	} else {
		data.buryState = BuryState::BURYING;
		data.ticks = data.duration;
		mPhysicsSystem->setRadius(physicsCmpnt, data.collisionRadius);
		mSpriteSystem->setSize(spriteCmpnt, data.size);
		mSpriteSystem->setSourceRect(spriteCmpnt, data.sourceRect);
		mSpriteSystem->animate(spriteCmpnt, false);
	}
}

//	============================================================================
void BurySystem::createComponent() {
	BuryComponentData data = {};
	data.buryState = BuryState::NORMAL;
	data.ticks = 0.0f;
	data.duration = 3.0f;

	mData.emplace_back(data);
}

//	============================================================================
void BurySystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
BuryComponent BurySystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void BurySystem::initialize(Random& random,
							PhysicsSystem& physicsSystem,
							SpriteSystem& spriteSystem) {
	mRandom = &random;
	mPhysicsSystem = &physicsSystem;
	mSpriteSystem = &spriteSystem;
}

//	============================================================================
void BurySystem::rise(const BuryComponent& cmpnt, bool immediate) {
	BuryComponentData& data = mData[cmpnt.index];

	const Entity& entity = this->getEntityByComponentIndex(cmpnt.index);
	SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);
	PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);

	if (data.buryState == BuryState::NORMAL) {
		data.collisionRadius = mPhysicsSystem->getRadius(physicsCmpnt);
		data.offsetY = mSpriteSystem->getOffsetY(spriteCmpnt);
		data.size = mSpriteSystem->getSize(spriteCmpnt);
		data.sourceRect = mSpriteSystem->getSourceRect(spriteCmpnt);
	}

	if (immediate) {
		data.buryState = BuryState::NORMAL;
		data.ticks = 0;
		mPhysicsSystem->setRadius(physicsCmpnt, data.collisionRadius);
		mSpriteSystem->setSize(spriteCmpnt, data.size);
		mSpriteSystem->setSourceRect(spriteCmpnt, data.sourceRect);
		mSpriteSystem->animate(spriteCmpnt, false);
	} else {
		data.buryState = BuryState::RISING;
		data.ticks = data.duration;
		mPhysicsSystem->setRadius(physicsCmpnt, data.collisionRadius);
		mSpriteSystem->setSize(spriteCmpnt, glm::vec2(0.0f, 0.0f));
		mSpriteSystem->setSourceRect(spriteCmpnt, data.sourceRect);
		mSpriteSystem->animate(spriteCmpnt, false);
	}
}

//	============================================================================
void BurySystem::setDuration(const BuryComponent& cmpnt, const float duration) {
	mData[cmpnt.index].duration = duration;
}

//	============================================================================
void BurySystem::setSpawnDirt(const BuryComponent& cmpnt, bool spawn) {
	mData[cmpnt.index].spawnDirt = spawn;
}

//	============================================================================
void BurySystem::spawnDirt(const glm::vec2 origin, const float progress) {
	const float chance = 100.0f - (100.0f * progress);

	if (mRandom->nextFloat(0.0f, chance) > (progress * 65.0f)) {
		const Entity entity = mEntityManager.createEntity();

		const float angle = mRandom->nextFloat(0.0f, TWO_PI);

		const float ellipseWidth = 8.0f;
		const float ellipseHeight = 4.0f;

		const float x = std::cos(angle) * ellipseWidth;
		const float y = std::sin(angle) * mRandom->nextFloat(0.0f, ellipseHeight);

		const float size = mRandom->nextFloat(0.25f, 2.5f);

		mPhysicsSystem->addComponent(entity);
		PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);
		mPhysicsSystem->setPosition(physicsCmpnt, origin + glm::vec2(x, y + 2.0f + size));
		mPhysicsSystem->setRadius(physicsCmpnt, 0);

		mSpriteSystem->addComponent(entity);
		SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);
		mSpriteSystem->setOffsetZ(spriteCmpnt, size * 2.f);
		mSpriteSystem->setTileset(spriteCmpnt, "dirt");
		mSpriteSystem->setTilesetId(spriteCmpnt, { mRandom->nextInt(0, 3) });
		mSpriteSystem->setSize(spriteCmpnt, glm::vec2(size));

		this->addComponent(entity);
		BuryComponent buryCmpnt = this->getComponent(entity);
		this->setDuration(buryCmpnt, 1.1f);
		this->setSpawnDirt(buryCmpnt, false);
		this->bury(buryCmpnt, true);
		this->rise(buryCmpnt, false);
	}
}

//	============================================================================
void BurySystem::update(float elapsedSeconds) {
	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;

		mData[index].ticks = std::max(
			mData[index].ticks - elapsedSeconds,
			0.0f);
	}

	for (const auto& p : mEntitiesByComponentIndices) {
		const size_t index = p.first;
		const Entity& entity = p.second;

		BuryComponentData& data = mData[index];

		const float d = data.duration;
		const float t = data.duration - data.ticks;

		const float progress = 1.0f - (data.ticks / data.duration);

		switch (data.buryState) {
			case BuryState::NORMAL: {
				break;
			}

			case BuryState::BURIED: {
				break;
			}

			case BuryState::BURYING: {
				SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);

				glm::vec2 size = data.size;
				size.y = std::round(easeOutCubic(t, size.y, -size.y, d));
				mSpriteSystem->setSize(spriteCmpnt, size);

				glm::ivec4 sourceRect = data.sourceRect;
				sourceRect[3] = (int)std::round(easeOutCubic(t, sourceRect[3], -sourceRect[3], d));
				mSpriteSystem->setSourceRect(spriteCmpnt, sourceRect);

				float diff = data.offsetY + (data.size.y - size.y) / 2;
				mSpriteSystem->setOffsetY(spriteCmpnt, diff);

				PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);

				if (data.spawnDirt) {
					glm::vec2 position = mPhysicsSystem->getPosition(physicsCmpnt);
					this->spawnDirt(position, progress);
				}

				if (data.ticks <= 0.0f) {
					mPhysicsSystem->setRadius(physicsCmpnt, data.collisionRadius);
					mSpriteSystem->animate(spriteCmpnt, true);
					data.buryState = BuryState::BURIED;
				}

				break;
			}

			case BuryState::RISING: {
				SpriteComponent spriteCmpnt = mSpriteSystem->getComponent(entity);

				glm::vec2 size = data.size;
				size.y = std::round(easeInCubic(t, 0.0f, size.y, d));
				mSpriteSystem->setSize(spriteCmpnt, size);

				glm::ivec4 sourceRect = data.sourceRect;
				sourceRect[3] = (int)std::round(easeInCubic(t, 0.0f, sourceRect[3], d));
				mSpriteSystem->setSourceRect(spriteCmpnt, sourceRect);

				float diff = data.offsetY + (data.size.y - size.y) / 2;
				mSpriteSystem->setOffsetY(spriteCmpnt, diff);

				PhysicsComponent physicsCmpnt = mPhysicsSystem->getComponent(entity);

				if (data.spawnDirt) {
					glm::vec2 position = mPhysicsSystem->getPosition(physicsCmpnt);
					this->spawnDirt(position, progress);
				}

				if (data.ticks <= 0.0f) {
					mSpriteSystem->animate(spriteCmpnt, true);
					mPhysicsSystem->setRadius(physicsCmpnt, data.collisionRadius);
					data.buryState = BuryState::NORMAL;
				}

				break;
			}
		}
	}
}
}