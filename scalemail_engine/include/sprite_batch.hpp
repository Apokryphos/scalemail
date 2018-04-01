#pragma once

#include "gl_headers.hpp"
#include "sprite_shader.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace ScaleMail
{
class AssetManager;
class Tileset;

class SpriteBatch
{
	static const int InitialSpriteCount = 500;
	static const int VertexElementCount      = 9;
	static const int QuadVertexCount         = 4;
	static const int QuadIndexCount          = 6;
	static const int QuadVertexElementCount  = QuadVertexCount * VertexElementCount;

	struct Batch {
		size_t indexOffset         = 0;
		size_t vertexElementOffset = 0;
		//	Used to keep track of current index buffer index
		unsigned short              vertexOffset      = 0;
		int                         quadCount         = 0;
		GLuint                      textureId         = 0;
		size_t                      indexElementCount = 0;
		std::vector<unsigned short> indexData;
		size_t                      vertexElementCount = 0;
		std::vector<GLfloat>        vertexData;

		Batch()
		: indexData(InitialSpriteCount * QuadIndexCount),
		  vertexData(InitialSpriteCount * QuadVertexElementCount) {
		}
	};

	float mAlpha;
	float mTileZ;

	std::string mName;

	SpriteShader mShader;
	GLuint   mVao;

	size_t mIndexBufferSizeInElements;
	GLuint mIndexBuffer;

	size_t mVertexBufferSizeInElements;
	GLuint mVertexBuffer;

	std::unordered_map<GLuint, Batch> mBatchByTexture;
	std::unordered_map<GLuint, Batch> mAlphaBatchByTexture;

	std::unordered_map<bool, std::unordered_map<unsigned int, int>>	mTextureIdCounts;

	inline void clearBatch(Batch& batch) {
		batch.quadCount           = 0;
		batch.indexOffset         = 0;
		batch.vertexOffset        = 0;
		batch.vertexElementOffset = 0;
		batch.indexElementCount   = 0;
		batch.vertexElementCount  = 0;
	}

	void renderBatches(std::unordered_map<GLuint, Batch> batches, bool alpha);
	void resizeBatchData(Batch& batch, size_t quadCount);

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
	void initialize(const std::string& spriteBatchName,
					AssetManager& assetManager);
	void setAlpha(float alpha);
	void render(const glm::mat4& transform);
};
}