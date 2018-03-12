#include "direction.hpp"
#include "direction_util.hpp"
#include "asset_manager.hpp"
#include "layer.hpp"
#include "physics_system.hpp"
#include "sprite.hpp"
#include "sprite_animation.hpp"
#include "sprite_batch.hpp"
#include "sprite_system.hpp"
#include "texture.hpp"
#include "tileset.hpp"
#include "vector_util.hpp"
#include <cmath>

namespace ScaleMail
{
//	============================================================================
static inline bool sortSprite(const SpriteComponentData& data1,
				const SpriteComponentData& data2) {
	return data1.position.z < data2.position.z;
}

//	============================================================================
static SpriteComponent makeComponent(const int index) {
	return SpriteComponent(index);
}

//	============================================================================
SpriteSystem::SpriteSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);
}

//	============================================================================
void SpriteSystem::animate(const SpriteComponent& cmpnt, const bool animate) {
	mData[cmpnt.index].animate = animate ? 1.0f : 0.0f;
}

//	============================================================================
void SpriteSystem::createComponent() {
	SpriteComponentData data = {};
	data.animate = 1.0f;
	data.facing = Direction::SOUTH;
	data.offsetY = 0.0f;
	data.decal = 0.0f;
	data.textureId = 0;
	data.maskTextureId = 0;
	data.tilesetId = 0;
	data.rotate = 0;
	data.rotateOffset = 0.0f;
	data.size = glm::vec2(16.0f);
	data.sourceRect = glm::ivec4(0, 0, 16, 16);
	data.uv1 = glm::vec2(0.0f);
	data.uv2 = glm::vec2(0.0f);
	data.position = glm::vec3(0.0f);
	data.color = glm::vec4(1.0f);
	data.maskColor = glm::vec4(1.0f);
	data.alpha = true;
	data.tilesetName = "";
	setSpriteAnimation(data.animation, { 0 });

	mData.emplace_back(data);
}

//	============================================================================
void SpriteSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
void SpriteSystem::buildVertexData(
	SpriteBatch& spriteBatch,
	const std::vector<SpriteComponentData>& spriteData,
	bool mask) {
	//	Size of component vectors is limited by int EntitySystem.MaxComponents
	const int spriteCount = static_cast<int>(spriteData.size());

	mAlpha.resize(0);
	mTextureId.resize(0);
	mPositionX.resize(0);
	mPositionY.resize(0);
	mPositionZ.resize(0);
	mColorR.resize(0);
	mColorG.resize(0);
	mColorB.resize(0);
	mColorA.resize(0);
	mSizeX.resize(0);
	mSizeY.resize(0);
	mRotate.resize(0);
	mTexU1.resize(0);
	mTexV1.resize(0);
	mTexU2.resize(0);
	mTexV2.resize(0);

	//	Sort sprites
	std::vector<SpriteComponentData> sprites(spriteData);
	std::sort(sprites.begin(), sprites.end(), sortSprite);

	//	Small offset to prevent sprites against walls from being hidden
	const float SPRITE_OFFSET = 1.0f;

	for (const auto& sprite : sprites) {
		mTextureId.emplace_back(mask ? sprite.maskTextureId : sprite.textureId);
		mAlpha.emplace_back(sprite.alpha);
		mPositionX.emplace_back(sprite.position.x);
		mPositionY.emplace_back(sprite.position.y + SPRITE_OFFSET);
		mPositionZ.emplace_back(sprite.position.z);
		mColorR.emplace_back(mask ? sprite.maskColor.r : sprite.color.r);
		mColorG.emplace_back(mask ? sprite.maskColor.g : sprite.color.g);
		mColorB.emplace_back(mask ? sprite.maskColor.b : sprite.color.b);
		mColorA.emplace_back(mask ? sprite.maskColor.a : sprite.color.a);
		mSizeX.emplace_back(sprite.size.x);
		mSizeY.emplace_back(sprite.size.y);
		mRotate.emplace_back(sprite.rotate + sprite.rotateOffset);
		mTexU1.emplace_back(sprite.uv1.x);
		mTexV1.emplace_back(sprite.uv1.y);
		mTexU2.emplace_back(sprite.uv2.x);
		mTexV2.emplace_back(sprite.uv2.y);
	}

	spriteBatch.buildSpriteVertexData(
		spriteCount,
		mTextureId,
		mAlpha,
		mPositionX,
		mPositionY,
		mPositionZ,
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
void SpriteSystem::buildVertexData(SpriteBatch& spriteBatch) {
	this->buildVertexData(spriteBatch, mData, false);
}

//	============================================================================
void SpriteSystem::buildVertexData(
	SpriteBatch& spriteBatch,
	const std::vector<Entity>& entities) {
	std::vector<SpriteComponentData> spriteData;

	for (const auto& entity : entities) {
		if (this->hasComponent(entity)) {
			const int index = this->getComponentIndexByEntity(entity);
			spriteData.emplace_back(mData[index]);
		}
	}

	this->buildVertexData(spriteBatch, spriteData, true);
}

//	============================================================================
bool SpriteSystem::getAlpha(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].alpha;
}

//	============================================================================
int SpriteSystem::getTilesetId(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].tilesetId;
}

//	============================================================================
SpriteComponent SpriteSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
float SpriteSystem::getOffsetY(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].offsetY;
}

//	============================================================================
float SpriteSystem::getRotate(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].rotate;
}

//	============================================================================
glm::vec2 SpriteSystem::getSize(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].size;
}

//	============================================================================
glm::ivec4 SpriteSystem::getSourceRect(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].sourceRect;
}

//	============================================================================
int SpriteSystem::getTextureId(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].textureId;
}

//	============================================================================
void SpriteSystem::calculateTextureCoords(int componentIndex) {
	glm::vec2 uv1;
	glm::vec2 uv2;

	mData[componentIndex].tileset.getTileUv(
		mData[componentIndex].tilesetId, mData[componentIndex].sourceRect, uv1, uv2);

	mData[componentIndex].uv1 = uv1;
	mData[componentIndex].uv2 = uv2;
}

//	============================================================================
void SpriteSystem::initialize(AssetManager& assetManager) {
	mAssetManager = &assetManager;
}

//	============================================================================
void SpriteSystem::setActorIndex(const SpriteComponent& cmpnt,
								 const int actorIndex) {
	getActorSpriteAnimation(actorIndex, mData[cmpnt.index].animation);
	this->updateAnimationTileset(cmpnt, 0);
}

//	============================================================================
void SpriteSystem::setAlpha(const SpriteComponent& cmpnt, const bool alpha) {
	mData[cmpnt.index].alpha = alpha;
}

//	============================================================================
void SpriteSystem::setAnimationDuration(const SpriteComponent& cmpnt,
										const float duration) {
	SpriteComponentData& data = mData[cmpnt.index];

	float frameDuration = duration / data.animation.frameCount;

	for (auto& frame : data.animation.frames) {
		frame.duration = frameDuration;
	}
}

//	============================================================================
void SpriteSystem::setColor(const SpriteComponent& cmpnt,
							const glm::vec4 color) {
	mData[cmpnt.index].color = color;
}

//	============================================================================
void SpriteSystem::setDecal(const SpriteComponent& cmpnt, bool decal) {
	mData[cmpnt.index].decal = decal ? 1.0f : 0.0f;
}

//	============================================================================
void SpriteSystem::setFacing(const SpriteComponent& cmpnt,
							 const Direction facing) {
	if (mData[cmpnt.index].facing != facing) {
		mData[cmpnt.index].facing = facing;
		this->updateAnimationTileset(cmpnt.index,
									mData[cmpnt.index].animation.frameIndex);
	}
}

//	============================================================================
void SpriteSystem::setDirection(
	const SpriteComponent& cmpnt, const glm::vec2 direction) {
//	Temporary adjustment to rotate sprite until tilesets can
//	be corrected.
#define ADJUST 4.712389f
	mData[cmpnt.index].rotate = (float)atan2(direction.x, -direction.y) + ADJUST;
}

//	============================================================================
void SpriteSystem::setMaskColor(const SpriteComponent& cmpnt,
							const glm::vec4 color) {
	mData[cmpnt.index].maskColor = color;
}

//	============================================================================
void SpriteSystem::setOffsetY(const SpriteComponent& cmpnt, float offsetY) {
	mData[cmpnt.index].offsetY = offsetY;
}

//	============================================================================
void SpriteSystem::setRotate(const SpriteComponent& cmpnt, float rotate) {
	mData[cmpnt.index].rotate = rotate;
}

//	============================================================================
void SpriteSystem::setRotateOffset(const SpriteComponent& cmpnt, float offset) {
	mData[cmpnt.index].rotateOffset = offset;
}

//	============================================================================
void SpriteSystem::setSize(
	const SpriteComponent& cmpnt, const glm::vec2& size) {
	mData[cmpnt.index].size = size;
}

//	============================================================================
void SpriteSystem::setSourceRect(
		const SpriteComponent& cmpnt, const glm::ivec4& sourceRect) {
	mData[cmpnt.index].sourceRect = sourceRect;
	this->calculateTextureCoords(cmpnt.index);
}

//	============================================================================
void SpriteSystem::setTileset(
	const SpriteComponent& cmpnt, const std::string& textureName) {
	SpriteComponentData& data = mData[cmpnt.index];

	Tileset tileset = mAssetManager->getTileset(textureName);

	Texture maskTexture = mAssetManager->loadTexture(textureName + "_mask");

	data.textureId = tileset.texture.id;
	data.maskTextureId = maskTexture.id;
	data.tileset = tileset;

	this->calculateTextureCoords(cmpnt.index);

	data.tilesetName = textureName;
}

//	============================================================================
void SpriteSystem::setTilesetId(const SpriteComponent& cmpnt,
								std::vector<int> frameTilesetIds) {
	assert(frameTilesetIds.size() > 0);
	mData[cmpnt.index].tilesetId = frameTilesetIds[0];
	this->calculateTextureCoords(cmpnt.index);
	setSpriteAnimation(mData[cmpnt.index].animation, frameTilesetIds);
}

//	============================================================================
void SpriteSystem::update(float elapsedSeconds, PhysicsSystem& physicsSystem) {
	//	Update sprite positions from position system
	glm::vec2 position;
	for (const auto& p : mEntitiesByComponentIndices) {
		position =
			physicsSystem.getPosition(physicsSystem.getComponent(p.second));

		const size_t index = p.first;

		mData[index].position.x = position.x;
		mData[index].position.y = position.y + mData[index].offsetY;
		mData[index].position.z = (1.0f - mData[index].decal);
	}

	for (const auto& p : mEntitiesByComponentIndices) {
		SpriteAnimation& animation = mData[p.first].animation;

		animation.ticks += elapsedSeconds * mData[p.first].animate;

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
	SpriteAnimation& animation = mData[cmpnt.index].animation;

	animation.frameIndex = frameIndex;

	if (animation.frameIndex >= animation.frameCount) {
		animation.frameIndex = 0;
	}

	const int direction = static_cast<int>(mData[cmpnt.index].facing);

	mData[cmpnt.index].tilesetId =
		animation.frames[animation.frameIndex].tilesetIds[direction];

	this->calculateTextureCoords(cmpnt.index);
}
}