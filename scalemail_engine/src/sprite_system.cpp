#include "sprite_system.hpp"
#include "direction.hpp"
#include "asset_manager.hpp"
#include "layer.hpp"
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
bool sortSprite(const SpriteComponentData& data1,
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
void SpriteSystem::createComponent() {
	SpriteComponentData data = {};
	data.facing = Direction::SOUTH;
	data.offsetZ = 16.0f;
	data.textureId = 0;
	data.tilesetId = 0;
	data.rotate = 0;
	data.size = glm::vec2(16.0f);
	data.uv1 = glm::vec2(0.0f);
	data.uv2 = glm::vec2(0.0f);
	data.position = glm::vec3(0.0f);
	data.color = glm::vec4(1.0f);
	data.alpha = true;
	data.tilesetName = "";
	setSpriteAnimation(data.animation, { 0 });

	mData.emplace_back(data);

	++mTextureIdCounts[data.alpha][data.textureId];
}

//	============================================================================
void SpriteSystem::destroyComponent(int index) {
	assert(mTextureIdCounts[mData[index].alpha][mData[index].textureId] > 0);
	--mTextureIdCounts[mData[index].alpha][mData[index].textureId];

	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
void SpriteSystem::buildVertexData(SpriteBatch& spriteBatch) {
	//	Size of component vectors is limited by int EntitySystem.MaxComponents
	const int spriteCount = static_cast<int>(mData.size());

	mAlpha.clear();
	mTextureId.clear();
	mPositionX.clear();
	mPositionY.clear();
	mPositionZ.clear();
	mColorR.clear();
	mColorG.clear();
	mColorB.clear();
	mColorA.clear();
	mSizeX.clear();
	mSizeY.clear();
	mRotate.clear();
	mTexU1.clear();
	mTexV1.clear();
	mTexU2.clear();
	mTexV2.clear();

	//	Sort sprites
	std::vector<SpriteComponentData> sprites(mData);
	std::sort(sprites.begin(), sprites.end(), sortSprite);

	for (const auto& sprite : sprites) {
		mTextureId.push_back(sprite.textureId);
		mAlpha.push_back(sprite.alpha);
		mPositionX.push_back(sprite.position.x);
		mPositionY.push_back(sprite.position.y);
		mPositionZ.push_back(sprite.position.z);
		mColorR.push_back(sprite.color.r);
		mColorG.push_back(sprite.color.g);
		mColorB.push_back(sprite.color.b);
		mColorA.push_back(sprite.color.a);
		mSizeX.push_back(sprite.size.x);
		mSizeY.push_back(sprite.size.y);
		mRotate.push_back(sprite.rotate);
		mTexU1.push_back(sprite.uv1.x);
		mTexV1.push_back(sprite.uv1.y);
		mTexU2.push_back(sprite.uv2.x);
		mTexV2.push_back(sprite.uv2.y);
	}

	spriteBatch.buildSpriteVertexData(
		spriteCount,
		mTextureIdCounts,
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
float SpriteSystem::getRotate(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].rotate;
}

//	============================================================================
glm::vec2 SpriteSystem::getSize(const SpriteComponent& cmpnt) const {
	return mData[cmpnt.index].size;
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
		mData[componentIndex].tilesetId, uv1, uv2);

	mData[componentIndex].uv1.x = uv1.x;
	mData[componentIndex].uv1.y = uv2.y;

	mData[componentIndex].uv2.x = uv2.x;
	mData[componentIndex].uv2.y = uv1.y;
}

//	============================================================================
void SpriteSystem::initialize(AssetManager* assetManager) {
	mAssetManager = assetManager;
}

//	============================================================================
void SpriteSystem::setActorIndex(const SpriteComponent& cmpnt,
								 const int actorIndex) {
	getActorSpriteAnimation(actorIndex, mData[cmpnt.index].animation);
}

//	============================================================================
void SpriteSystem::setAlpha(const SpriteComponent& cmpnt, const bool alpha) {
	SpriteComponentData& data = mData[cmpnt.index];
	--mTextureIdCounts[data.alpha][data.textureId];
	data.alpha = alpha;
	++mTextureIdCounts[data.alpha][data.textureId];
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
void SpriteSystem::setFacing(const SpriteComponent& cmpnt,
							 const Direction facing) {
	mData[cmpnt.index].facing = facing;
	this->updateAnimationTileset(cmpnt.index,
								 mData[cmpnt.index].animation.frameIndex);
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
void SpriteSystem::setOffsetZ(const SpriteComponent& cmpnt, float offsetZ) {
	mData[cmpnt.index].offsetZ = offsetZ;
}

//	============================================================================
void SpriteSystem::setRotate(const SpriteComponent& cmpnt, float rotate) {
	mData[cmpnt.index].rotate = rotate;
}

//	============================================================================
void SpriteSystem::setSize(
	const SpriteComponent& cmpnt, const glm::vec2& size) {
	mData[cmpnt.index].size = size;
}

//	============================================================================
void SpriteSystem::setTileset(
	const SpriteComponent& cmpnt, const std::string& textureName) {
	SpriteComponentData& data = mData[cmpnt.index];

	Tileset tileset = mAssetManager->getTileset(textureName);

	--mTextureIdCounts[data.alpha][data.textureId];
	data.textureId = tileset.texture.id;
	++mTextureIdCounts[data.alpha][data.textureId];

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
	for (auto& p : mEntitiesByComponentIndices) {
		position =
			physicsSystem.getPosition(physicsSystem.getComponent(p.second));

		size_t index = p.first;

		mData[index].position.x = position.x;
		mData[index].position.y = position.y;

		mData[index].position.z = getLayerZ(2,
			(position.y - mData[index].size.y * 0.5f) + mData[index].offsetZ);
	}

	for (auto& p : mEntitiesByComponentIndices) {
		SpriteAnimation& animation = mData[p.first].animation;

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