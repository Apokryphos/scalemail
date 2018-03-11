#pragma once

#include "gl_headers.hpp"
#include "sprite_shader.hpp"
#include <glm/fwd.hpp>
#include <unordered_map>
#include <vector>

namespace ScaleMail
{
class AssetManager;
struct RenderOptions;
class Tileset;

class SpriteBatch
{
	struct Batch {
		size_t IndexOffset         = 0;
		size_t VertexElementOffset = 0;
		//	Used to keep track of current index buffer index
		unsigned short              VertexOffset      = 0;
		int                         QuadCount         = 0;
		GLuint                      TextureId         = 0;
		size_t                      IndexElementCount = 0;
		std::vector<unsigned short> IndexData;
		size_t                      VertexElementCount = 0;
		std::vector<GLfloat>        VertexData;
	};

	const int InitialVboSizeInSprites = 500;
	const int VertexElementCount      = 9;
	const int QuadVertexCount         = 4;
	const int QuadIndexCount          = 6;
	const int QuadVertexElementCount  = QuadVertexCount * VertexElementCount;

	float mAlpha = 1.0f;

	SpriteShader mShader;
	GLuint   mVao;

	size_t mIndexBufferSizeInElements = 0;
	GLuint mIndexBuffer;

	size_t mVertexBufferSizeInElements = 0;
	GLuint mVertexBuffer;

	std::unordered_map<GLuint, Batch> mBatchByTexture;
	std::unordered_map<GLuint, Batch> mAlphaBatchByTexture;

	std::unordered_map<bool, std::unordered_map<unsigned int, int>>	mTextureIdCounts;

	inline void clearBatch(Batch& batch) {
		batch.QuadCount           = 0;
		batch.IndexOffset         = 0;
		batch.VertexOffset        = 0;
		batch.VertexElementOffset = 0;
		batch.IndexElementCount   = 0;
		batch.VertexElementCount  = 0;
	}

	void renderBatches(std::unordered_map<GLuint, Batch> batches, bool alpha);

public:
	SpriteBatch();
	SpriteBatch(const SpriteBatch&) = delete;
	SpriteBatch& operator=(const SpriteBatch&) = delete;

	void begin();

	void buildTileVertexData(
		int textureId, const glm::vec2& position, const glm::vec2& size,
		const glm::vec2& uv1, const glm::vec2& uv2, bool alpha);

	void buildQuadVertexData(
		int textureId, const std::vector<glm::vec3>& position,
		const std::vector<glm::vec2>& size, const std::vector<glm::vec4>& color,
		bool alpha);

	void buildSpriteVertexData(
		const int spriteCount,
		const std::vector<unsigned int>& textureId,
		const std::vector<bool>& alpha, const std::vector<float>& positionX,
		const std::vector<float>& positionY, const std::vector<float>& positionZ,
		const std::vector<float>& colorR,
		const std::vector<float>& colorG, const std::vector<float>& colorB,
		const std::vector<float>& colorA, const std::vector<float>& sizeX,
		const std::vector<float>& sizeY, const std::vector<float>& rotate,
		const std::vector<float>& texU1, const std::vector<float>& texV1,
		const std::vector<float>& texU2,
		const std::vector<float>& texV2);

	void buildTileVertexData(
		Tileset& tileset, int tilesetId, const glm::vec2& position,
		const glm::vec2& size, bool alpha);

	void end();
	void initialize(AssetManager& assetManager,
					const RenderOptions& renderOptions);
	void setAlpha(float alpha);
	void render(const glm::mat4& transform);
};
}