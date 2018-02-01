#include "sprite_system.hpp"
#include "direction.hpp"
#include "asset_manager.hpp"
#include "physics_system.hpp"
#include "sprite.hpp"
#include "sprite_animation.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include "tileset.hpp"
#include "vector_util.hpp"
#include <cmath>

namespace ScaleMail
{
//	============================================================================
static SpriteComponent makeComponent(const int index) {
	return SpriteComponent(index);
}

//	============================================================================
SpriteSystem::SpriteSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mFacing.reserve(maxComponents);
	mTilesetId.reserve(maxComponents);
	mAlpha.reserve(maxComponents);
	mTextureId.reserve(maxComponents);
	mPositionX.reserve(maxComponents);
	mPositionY.reserve(maxComponents);
	mColorR.reserve(maxComponents);
	mColorG.reserve(maxComponents);
	mColorB.reserve(maxComponents);
	mColorA.reserve(maxComponents);
	mSizeX.reserve(maxComponents);
	mSizeY.reserve(maxComponents);
	mRotate.reserve(maxComponents);
	mTexU1.reserve(maxComponents);
	mTexV1.reserve(maxComponents);
	mTexU2.reserve(maxComponents);
	mTexV2.reserve(maxComponents);
	mTilesets.reserve(maxComponents);
	mTilesetName.reserve(maxComponents);
	mAnimation.reserve(maxComponents);
}

//	============================================================================
void SpriteSystem::createComponent() {
	mFacing.emplace_back(Direction::SOUTH);
	mTilesetId.emplace_back(0);
	mAlpha.emplace_back(true); //	Enable alpha by default
	mTextureId.emplace_back(0);
	mPositionX.emplace_back(0.0f);
	mPositionY.emplace_back(0.0f);
	mColorR.emplace_back(1.0f);
	mColorG.emplace_back(1.0f);
	mColorB.emplace_back(1.0f);
	mColorA.emplace_back(1.0f);
	mSizeX.emplace_back(16.0f);
	mSizeY.emplace_back(16.0f);
	mRotate.emplace_back(0.0f);
	mTexU1.emplace_back(0.0f);
	mTexV1.emplace_back(0.0f);
	mTexU2.emplace_back(0.0f);
	mTexV2.emplace_back(0.0f);
	mTilesets.emplace_back();
	mTilesetName.emplace_back("");

	SpriteAnimation spriteAnimation;
	setSpriteAnimation(spriteAnimation, 0, 0);
	mAnimation.push_back(spriteAnimation);

	++mTextureIdCounts[mAlpha.back()][mTextureId.back()];
}

//	============================================================================
void SpriteSystem::destroyComponent(int index) {
	assert(mTextureIdCounts[mAlpha[index]][mTextureId[index]] > 0);
	--mTextureIdCounts[mAlpha[index]][mTextureId[index]];

	swapWithLastElementAndRemove(mTilesetId, index);
	swapWithLastElementAndRemove(mAlpha, index);
	swapWithLastElementAndRemove(mTextureId, index);
	swapWithLastElementAndRemove(mPositionX, index);
	swapWithLastElementAndRemove(mPositionY, index);
	swapWithLastElementAndRemove(mSizeX, index);
	swapWithLastElementAndRemove(mSizeY, index);
	swapWithLastElementAndRemove(mRotate, index);
	swapWithLastElementAndRemove(mTexU1, index);
	swapWithLastElementAndRemove(mTexV1, index);
	swapWithLastElementAndRemove(mTexU2, index);
	swapWithLastElementAndRemove(mTexV2, index);
	swapWithLastElementAndRemove(mTilesets, index);
	swapWithLastElementAndRemove(mTilesetName, index);
}

//	============================================================================
void SpriteSystem::buildVertexData(SpriteBatch& spriteBatch) {
	//	Size of component vectors is limited by int EntitySystem.MaxComponents
	const int spriteCount = static_cast<int>(mTilesetId.size());

	spriteBatch.buildSpriteVertexData(
		spriteCount,
		mTextureIdCounts,
		mTextureId,
		mAlpha,
		mPositionX,
		mPositionY,
		mColorR,
		mColorG,
		mColorB,
		mColorA,
		mSizeX,
		mSizeY,
		mRotate,
		mTexU1,
		mTexV1,
		mTexU2,
		mTexV2);
}

//	============================================================================
bool SpriteSystem::getAlpha(const SpriteComponent& cmpnt) const {
	return mAlpha[cmpnt.index];
}

//	============================================================================
int SpriteSystem::getTilesetId(const SpriteComponent& cmpnt) const {
	return mTilesetId[cmpnt.index];
}

//	============================================================================
SpriteComponent SpriteSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
float SpriteSystem::getRotate(const SpriteComponent& cmpnt) const {
	return mRotate[cmpnt.index];
}

//	============================================================================
glm::vec2 SpriteSystem::getSize(const SpriteComponent& cmpnt) const {
	return glm::vec2(mSizeX[cmpnt.index], mSizeY[cmpnt.index]);
}

//	============================================================================
int SpriteSystem::getTextureId(const SpriteComponent& cmpnt) const {
	return mTextureId[cmpnt.index];
}

//	============================================================================
void SpriteSystem::calculateTextureCoords(int componentIndex) {
	glm::vec2 uv1;
	glm::vec2 uv2;

	mTilesets[componentIndex].getTileUv(mTilesetId[componentIndex], uv1, uv2);

	mTexU1[componentIndex] = uv1.x;
	mTexV1[componentIndex] = uv2.y;
	mTexU2[componentIndex] = uv2.x;
	mTexV2[componentIndex] = uv1.y;
}

//	============================================================================
void SpriteSystem::initialize(AssetManager* assetManager) {
	mAssetManager = assetManager;
}

//	============================================================================
void SpriteSystem::setActorIndex(const SpriteComponent& cmpnt,
								 const int actorIndex) {
	getActorSpriteAnimation(actorIndex, mAnimation[cmpnt.index]);
}

//	============================================================================
void SpriteSystem::setAlpha(const SpriteComponent& cmpnt, const bool alpha) {
	--mTextureIdCounts[mAlpha[cmpnt.index]][mTextureId[cmpnt.index]];
	mAlpha[cmpnt.index] = alpha;
	++mTextureIdCounts[mAlpha[cmpnt.index]][mTextureId[cmpnt.index]];
}

//	============================================================================
void SpriteSystem::setFacing(const SpriteComponent& cmpnt,
							 const Direction facing) {
	mFacing[cmpnt.index] = facing;
	this->updateAnimationTileset(cmpnt.index, mAnimation[cmpnt.index].frameIndex);
}

//	============================================================================
void SpriteSystem::setDirection(
	const SpriteComponent& cmpnt, const glm::vec2 direction) {
//	Temporary adjustment to rotate sprite until tilesets can
//	be corrected.
#define ADJUST 4.712389f
	mRotate[cmpnt.index] = (float)atan2(direction.x, -direction.y) + ADJUST;
}

//	============================================================================
void SpriteSystem::setRotate(const SpriteComponent& cmpnt, float rotate) {
	mRotate[cmpnt.index] = rotate;
}

//	============================================================================
void SpriteSystem::setSize(
	const SpriteComponent& cmpnt, const glm::vec2& size) {
	mSizeX[cmpnt.index] = size.x;
	mSizeY[cmpnt.index] = size.y;
}

//	============================================================================
void SpriteSystem::setTileset(
	const SpriteComponent& cmpnt, const std::string& textureName) {

	Tileset tileset = mAssetManager->getTileset(textureName);

	--mTextureIdCounts[mAlpha[cmpnt.index]][mTextureId[cmpnt.index]];
	mTextureId[cmpnt.index] = tileset.texture.id;
	++mTextureIdCounts[mAlpha[cmpnt.index]][mTextureId[cmpnt.index]];

	mTilesets[cmpnt.index] = tileset;

	this->calculateTextureCoords(cmpnt.index);

	mTilesetName[cmpnt.index] = textureName;
}

//	============================================================================
void SpriteSystem::setTilesetId(const SpriteComponent& cmpnt,
								const int tilesetId) {
	this->setTilesetId(cmpnt, tilesetId, tilesetId);
}

//	============================================================================
void SpriteSystem::setTilesetId(const SpriteComponent& cmpnt,
								const int frame1TilesetId,
								const int frame2TilesetId) {
	mTilesetId[cmpnt.index] = frame1TilesetId;
	this->calculateTextureCoords(cmpnt.index);
	setSpriteAnimation(mAnimation[cmpnt.index], frame1TilesetId, frame2TilesetId);
}

//	============================================================================
void SpriteSystem::update(float elapsedSeconds, PhysicsSystem& physicsSystem) {
	//	Update sprite positions from position system
	glm::vec2 position;
	for (auto& p : mEntitiesByComponentIndices) {
		position =
			physicsSystem.getPosition(physicsSystem.getComponent(p.second));

		mPositionX[p.first] = position.x;
		mPositionY[p.first] = position.y;
	}

	for (auto& p : mEntitiesByComponentIndices) {
		SpriteAnimation& animation = mAnimation[p.first];

		animation.ticks += elapsedSeconds;

		const float duration = animation.frames[animation.frameIndex].duration;

		if (animation.ticks >= duration) {
			animation.ticks -= duration;
			this->updateAnimationTileset(p.first, animation.frameIndex + 1);
		}
	}
}

//	============================================================================
void SpriteSystem::updateAnimationTileset(const SpriteComponent& cmpnt,
										  int frameIndex) {
	SpriteAnimation& animation = mAnimation[cmpnt.index];

	animation.frameIndex = frameIndex;

	if (animation.frameIndex > 1) {
		animation.frameIndex = 0;
	}

	const int direction = static_cast<int>(mFacing[cmpnt.index]);

	mTilesetId[cmpnt.index] =
		animation.frames[animation.frameIndex].tilesetIds[direction];

	this->calculateTextureCoords(cmpnt.index);
}
}