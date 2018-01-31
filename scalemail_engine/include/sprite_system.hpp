#pragma once

#include "direction.hpp"
#include "entity_system.hpp"
#include "sprite_animation.hpp"
#include "tileset.hpp"
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ScaleMail
{
class AssetManager;
struct Entity;
class EntityManager;
class PhysicsSystem;
class SpriteBatch;

struct SpriteComponent {
	SpriteComponent(const int index) { this->index = index; }
	int index;
};

class SpriteSystem : public EntitySystem
{
private:
	const int VertexElementCount = 4;

	std::vector<Direction> mFacing;

	std::vector<float> mPositionX;
	std::vector<float> mPositionY;
	std::vector<float> mColorR;
	std::vector<float> mColorG;
	std::vector<float> mColorB;
	std::vector<float> mColorA;
	std::vector<float> mSizeX;
	std::vector<float> mSizeY;
	std::vector<float> mRotate;
	std::vector<float> mTexU1;
	std::vector<float> mTexV1;
	std::vector<float> mTexU2;
	std::vector<float> mTexV2;

	std::vector<SpriteAnimation> mAnimation;

	std::vector<unsigned int> mTextureId;
	std::vector<int>          mTilesetId;
	std::vector<std::string>  mTilesetName;
	std::vector<bool>         mAlpha;

	std::vector<Tileset> mTilesets;

	std::unordered_map<bool, std::unordered_map<unsigned int, int>>
		mTextureIdCounts;

	AssetManager* mAssetManager;

protected:
	void calculateTextureCoords(int componentIndex);
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;
	void updateAnimationTileset(const SpriteComponent& cmpnt, int frameIndex);

public:
	SpriteSystem(EntityManager& entityManager, int maxComponents = 10000);
	void buildVertexData(SpriteBatch& spriteBatch);
	bool getAlpha(const SpriteComponent& cmpnt) const;
	SpriteComponent getComponent(const Entity& entity) const;
	glm::vec2 getSize(const SpriteComponent& cmpnt) const;
	float getRotate(const SpriteComponent& cmpnt) const;
	int getTextureId(const SpriteComponent& cmpnt) const;
	int getTilesetId(const SpriteComponent& cmpnt) const;
	void initialize(AssetManager* assetManager);
	void setActorIndex(const SpriteComponent& cmpnt, const int actorIndex);
	void setAlpha(const SpriteComponent& cmpnt, const bool alpha);
	void setFacing(const SpriteComponent& cmpnt, const Direction facing);
	void setTilesetId(const SpriteComponent& cmpnt, const int assetId);
	void setDirection(
		const SpriteComponent& cmpnt, const glm::vec2 direction);
	void setRotate(const SpriteComponent& cmpnt, float rotate);
	void setSize(const SpriteComponent& cmpnt, const glm::vec2& size);
	void setTileset(
		const SpriteComponent& cmpnt, const std::string& textureName);
	void update(float elapsedSeconds, PhysicsSystem& physicsSystem);
};
}