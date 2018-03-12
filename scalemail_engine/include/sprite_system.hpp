#pragma once

#include "direction.hpp"
#include "entity_system.hpp"
#include "sprite_animation.hpp"
#include "tileset.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
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

struct SpriteComponentData {
	Direction facing;
	unsigned int textureId;
	unsigned int maskTextureId;
	int tilesetId;
	float animate;
	//	Decals are flat Y axis aligned quads
	float decal;
	float offsetY;
	float rotate;
	float rotateOffset;
	glm::vec2 size;
	glm::ivec4 sourceRect;
	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec3 position;
	glm::vec4 color;
	glm::vec4 maskColor;
	bool alpha;
	std::string tilesetName;
	SpriteAnimation animation;
	Tileset tileset;
};

class SpriteSystem : public EntitySystem
{
private:
	const int VertexElementCount = 4;

	std::vector<SpriteComponentData> mData;

	std::vector<unsigned int> mTextureId;
	std::vector<bool>  mAlpha;
	std::vector<float> mPositionX;
	std::vector<float> mPositionY;
	std::vector<float> mPositionZ;
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

	AssetManager* mAssetManager;

protected:
	void buildVertexData(
		SpriteBatch& spriteBatch,
		const std::vector<SpriteComponentData>& spriteData,
		bool mask);
	void calculateTextureCoords(int componentIndex);
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;
	void updateAnimationTileset(const SpriteComponent& cmpnt, int frameIndex);

public:
	SpriteSystem(EntityManager& entityManager, int maxComponents = 10000);
	void animate(const SpriteComponent& cmpnt, const bool animate);
	void buildVertexData(SpriteBatch& spriteBatch);
	void buildVertexData(
		SpriteBatch& spriteBatch,
		const std::vector<Entity>& entities);
	bool getAlpha(const SpriteComponent& cmpnt) const;
	SpriteComponent getComponent(const Entity& entity) const;
	float getOffsetY(const SpriteComponent& cmpnt) const;
	glm::vec2 getSize(const SpriteComponent& cmpnt) const;
	float getRotate(const SpriteComponent& cmpnt) const;
	glm::ivec4 getSourceRect(const SpriteComponent& cmpnt) const;
	int getTextureId(const SpriteComponent& cmpnt) const;
	int getTilesetId(const SpriteComponent& cmpnt) const;
	void initialize(AssetManager& assetManager);
	void setActorIndex(const SpriteComponent& cmpnt, const int actorIndex);
	void setAlpha(const SpriteComponent& cmpnt, const bool alpha);
	void setAnimationDuration(const SpriteComponent& cmpnt,
							  const float duration);
	void setColor(const SpriteComponent& cmpnt, const glm::vec4 color);
	void setDecal(const SpriteComponent& cmpnt, const bool decal);
	void setFacing(const SpriteComponent& cmpnt, const Direction facing);
	void setMaskColor(const SpriteComponent& cmpnt, const glm::vec4 color);
	void setOffsetY(const SpriteComponent& cmpnt, float offsetY);
	void setTilesetId(const SpriteComponent& cmpnt,
					  std::vector<int> frameTilesetIds);
	void setDirection(
		const SpriteComponent& cmpnt, const glm::vec2 direction);
	void setRotate(const SpriteComponent& cmpnt, float rotate);
	void setRotateOffset(const SpriteComponent& cmpnt, float offset);
	void setSize(const SpriteComponent& cmpnt, const glm::vec2& size);
	void setSourceRect(
		const SpriteComponent& cmpnt,
		const glm::ivec4& sourceRect);
	void setTileset(
		const SpriteComponent& cmpnt, const std::string& textureName);
	void update(float elapsedSeconds, PhysicsSystem& physicsSystem);
};
}