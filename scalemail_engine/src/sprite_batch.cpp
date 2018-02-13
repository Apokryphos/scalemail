#include "asset_manager.hpp"
#include "gl_headers.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace ScaleMail
{
static const glm::vec2 gQuadVertices[] = {
	glm::vec2(-0.5f, -0.5f),
	glm::vec2(-0.5f,  0.5f),
	glm::vec2( 0.5f,  0.5f),
	glm::vec2( 0.5f, -0.5f),
};

//	===========================================================================
SpriteBatch::SpriteBatch() {}

//	===========================================================================
void SpriteBatch::begin() {
	//	Reset texture ID counts
	mTextureIdCounts.clear();

	//	Reset batch counters
	for (auto& b : mBatchByTexture) {
		this->clearBatch(b.second);
	}
	for (auto& b : mAlphaBatchByTexture) {
		this->clearBatch(b.second);
	}
}

//	===========================================================================
void SpriteBatch::buildQuadVertexData(
	int textureId, const std::vector<glm::vec3>& position,
	const std::vector<glm::vec2>& size, const std::vector<glm::vec4>& color,
	bool alpha) {
	int quadCount = position.size();

	//	Calculate vertex buffer sizes
	Batch& batch =
		alpha ? mAlphaBatchByTexture[textureId] : mBatchByTexture[textureId];

	batch.QuadCount += quadCount;

	//	Assign texture ID in case batch was created
	batch.TextureId = textureId;

	//	Increase vertex and index buffers to size of largest batch
	batch.VertexElementCount += quadCount * QuadVertexElementCount;
	if (batch.VertexElementCount > mVertexBufferSizeInElements) {
		mVertexBufferSizeInElements = batch.VertexElementCount;
	}

	batch.IndexElementCount += quadCount * QuadIndexCount;
	if (batch.IndexElementCount > mIndexBufferSizeInElements) {
		mIndexBufferSizeInElements = batch.IndexElementCount;
	}

	if (batch.VertexElementCount > batch.VertexData.size()) {
		std::cout << "SpriteBatch resized vertex data." << std::endl;
		batch.VertexData.resize(batch.VertexElementCount * 2);
	}

	if (batch.IndexElementCount > batch.IndexData.size()) {
		std::cout << "SpriteBatch resized index data." << std::endl;
		batch.IndexData.resize(batch.IndexElementCount * 2);
	}

	//	Build vertex data
	for (int s = 0; s < quadCount; s++) {
		Batch& batch = alpha ? mAlphaBatchByTexture.at(textureId)
							 : mBatchByTexture.at(textureId);

		std::vector<float>&          vertexData = batch.VertexData;
		std::vector<unsigned short>& indexData  = batch.IndexData;

		unsigned short& vertex = batch.VertexOffset;
		size_t&         e      = batch.IndexOffset;
		size_t&         v      = batch.VertexElementOffset;

		glm::vec2 quadA = gQuadVertices[0];
		glm::vec2 quadB = gQuadVertices[1];
		glm::vec2 quadC = gQuadVertices[2];
		glm::vec2 quadD = gQuadVertices[3];

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadA.x;
		vertexData[v++] = position[s].y + size[s].y * quadA.y;
		vertexData[v++] = position[s].z;
		//	UV
		vertexData[v++] = 0.0f;
		vertexData[v++] = 1.0f;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadB.x;
		vertexData[v++] = position[s].y + size[s].y * quadB.y;
		vertexData[v++] = position[s].z;
		//	UV
		vertexData[v++] = 0.0f;
		vertexData[v++] = 0.0f;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadC.x;
		vertexData[v++] = position[s].y + size[s].y * quadC.y;
		vertexData[v++] = position[s].z;
		//	UV
		vertexData[v++] = 1.0f;
		vertexData[v++] = 0.0f;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadD.x;
		vertexData[v++] = position[s].y + size[s].y * quadD.y;
		vertexData[v++] = position[s].z;
		//	UV
		vertexData[v++] = 1.0f;
		vertexData[v++] = 1.0f;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;

		//	Indices
		indexData[e++] = vertex;
		indexData[e++] = vertex + 1;
		indexData[e++] = vertex + 2;
		indexData[e++] = vertex;
		indexData[e++] = vertex + 2;
		indexData[e++] = vertex + 3;
		vertex += 4;
	}
}

//	===========================================================================
void SpriteBatch::buildTileVertexData(
	Tileset& tileset, int tilesetId, const glm::vec2& position,
	const glm::vec2& size, bool alpha) {
	Batch& batch = alpha ? mAlphaBatchByTexture[tileset.texture.id]
						 : mBatchByTexture[tileset.texture.id];

	++batch.QuadCount;

	//	Assign texture ID in case batch was created
	batch.TextureId = tileset.texture.id;

	//	Increase vertex and index buffers to size of largest batch
	batch.VertexElementCount += QuadVertexElementCount;
	if (batch.VertexElementCount > mVertexBufferSizeInElements) {
		mVertexBufferSizeInElements = batch.VertexElementCount;
	}

	batch.IndexElementCount += QuadIndexCount;
	if (batch.IndexElementCount > mIndexBufferSizeInElements) {
		mIndexBufferSizeInElements = batch.IndexElementCount;
	}

	if (batch.VertexElementCount > batch.VertexData.size()) {
		std::cout <<  "SpriteBatch resized vertex data." << std::endl;
		batch.VertexData.resize(batch.VertexElementCount * 2);
	}

	if (batch.IndexElementCount > batch.IndexData.size()) {
		std::cout << "SpriteBatch resized index data." << std::endl;
		batch.IndexData.resize(batch.IndexElementCount * 2);
	}

	std::vector<float>&          vertexData = batch.VertexData;
	std::vector<unsigned short>& indexData  = batch.IndexData;

	unsigned short& vertex = batch.VertexOffset;
	size_t&         e      = batch.IndexOffset;
	size_t&         v      = batch.VertexElementOffset;

	glm::vec2 quadA = gQuadVertices[0];
	glm::vec2 quadB = gQuadVertices[1];
	glm::vec2 quadC = gQuadVertices[2];
	glm::vec2 quadD = gQuadVertices[3];

	glm::vec2 uv1;
	glm::vec2 uv2;
	tileset.getTileUv(tilesetId, uv1, uv2);

	//	Position
	vertexData[v++] = position.x + size.x * quadA.x;
	vertexData[v++] = position.y + size.y * quadA.y;
	vertexData[v++] = 0.0f;
	//	UV
	vertexData[v++] = uv1.x;
	vertexData[v++] = uv2.y;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;

	//	Position
	vertexData[v++] = position.x + size.x * quadB.x;
	vertexData[v++] = position.y + size.y * quadB.y;
	vertexData[v++] = 0.0f;
	//	UV
	vertexData[v++] = uv1.x;
	vertexData[v++] = uv1.y;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;

	//	Position
	vertexData[v++] = position.x + size.x * quadC.x;
	vertexData[v++] = position.y + size.y * quadC.y;
	vertexData[v++] = 0.0f;
	//	UV
	vertexData[v++] = uv2.x;
	vertexData[v++] = uv1.y;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;

	//	Position
	vertexData[v++] = position.x + size.x * quadD.x;
	vertexData[v++] = position.y + size.y * quadD.y;
	vertexData[v++] = 0.0f;
	//	UV
	vertexData[v++] = uv2.x;
	vertexData[v++] = uv2.y;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;

	indexData[e++] = vertex;
	indexData[e++] = vertex + 1;
	indexData[e++] = vertex + 2;
	indexData[e++] = vertex;
	indexData[e++] = vertex + 2;
	indexData[e++] = vertex + 3;
	vertex += 4;
}

//	===========================================================================
void SpriteBatch::buildSpriteVertexData(
	const int spriteCount,
	const std::vector<unsigned int>& textureId,
	const std::vector<bool>& alpha, const std::vector<float>& positionX,
	const std::vector<float>& positionY, const std::vector<float>& positionZ,
	const std::vector<float>& colorR,
	const std::vector<float>& colorG, const std::vector<float>& colorB,
	const std::vector<float>& colorA, const std::vector<float>& sizeX,
	const std::vector<float>& sizeY, const std::vector<float>& rotate,
	const std::vector<float>& texU1, const std::vector<float>& texV1,
	const std::vector<float>& texU2, const std::vector<float>& texV2) {
	assert(textureId.size() == alpha.size());

	//	Calculate total quads per texture ID
	const size_t textureIdCount = textureId.size();
	for (size_t n = 0; n < textureIdCount; ++n) {
		++mTextureIdCounts[alpha[n]][textureId[n]];
	}

	//	Calculate vertex buffer sizes
	for (auto boolMap : mTextureIdCounts) {
		bool alpha = boolMap.first;

		for (auto p : boolMap.second) {
			Batch& batch = alpha ? mAlphaBatchByTexture[p.first]
								 : mBatchByTexture[p.first];

			batch.QuadCount += p.second;

			//	Assign texture ID in case batch was created
			batch.TextureId = p.first;

			//	Increase vertex and index buffers to size of largest batch
			batch.VertexElementCount += p.second * QuadVertexElementCount;
			if (batch.VertexElementCount > mVertexBufferSizeInElements) {
				mVertexBufferSizeInElements = batch.VertexElementCount;
			}

			batch.IndexElementCount += p.second * QuadIndexCount;
			if (batch.IndexElementCount > mIndexBufferSizeInElements) {
				mIndexBufferSizeInElements = batch.IndexElementCount;
			}

			if (batch.VertexElementCount > batch.VertexData.size()) {
				std::cout << "SpriteBatch resized vertex data." << std::endl;
				batch.VertexData.resize(batch.VertexElementCount * 2);
			}

			if (batch.IndexElementCount > batch.IndexData.size()) {
				std::cout << "SpriteBatch resized index data." << std::endl;
				batch.IndexData.resize(batch.IndexElementCount * 2);
			}
		}
	}

	//	Build vertex data
	for (int s = 0; s < spriteCount; s++) {
		Batch& batch = alpha[s] ? mAlphaBatchByTexture.at(textureId[s])
								: mBatchByTexture.at(textureId[s]);

		std::vector<float>&          vertexData = batch.VertexData;
		std::vector<unsigned short>& indexData  = batch.IndexData;

		unsigned short& vertex = batch.VertexOffset;
		size_t&         e      = batch.IndexOffset;
		size_t&         v      = batch.VertexElementOffset;

		//	Rotation code should be moved into shader
		float sinAngle = sin(rotate[s]);
		float cosAngle = cos(rotate[s]);

		glm::mat2 rotation =
			glm::mat2(cosAngle, sinAngle, -sinAngle, cosAngle);

		glm::vec2 quadA = rotation * gQuadVertices[0];
		glm::vec2 quadB = rotation * gQuadVertices[1];
		glm::vec2 quadC = rotation * gQuadVertices[2];
		glm::vec2 quadD = rotation * gQuadVertices[3];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadA.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadA.y;
		vertexData[v++] = positionZ[s];
		//	UV
		vertexData[v++] = texU1[s];
		vertexData[v++] = texV2[s];
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadB.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadB.y;
		vertexData[v++] = positionZ[s];
		//	UV
		vertexData[v++] = texU1[s];
		vertexData[v++] = texV1[s];
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadC.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadC.y;
		vertexData[v++] = positionZ[s];
		//	UV
		vertexData[v++] = texU2[s];
		vertexData[v++] = texV1[s];
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadD.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadD.y;
		vertexData[v++] = positionZ[s];
		//	UV
		vertexData[v++] = texU2[s];
		vertexData[v++] = texV2[s];
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];

		indexData[e++] = vertex;
		indexData[e++] = vertex + 1;
		indexData[e++] = vertex + 2;
		indexData[e++] = vertex;
		indexData[e++] = vertex + 2;
		indexData[e++] = vertex + 3;
		vertex += 4;
	}
}

//	===========================================================================
void SpriteBatch::end() {}

//	===========================================================================
void SpriteBatch::initialize(AssetManager& assetManager) {
	const GLsizei stride =
		static_cast<GLsizei>(VertexElementCount * sizeof(GLfloat));

	mShader = assetManager.getSpriteShader();

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVertexBuffer);
	glGenBuffers(1, &mIndexBuffer);

	glBindVertexArray(mVao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			InitialVboSizeInSprites * QuadIndexCount *
			sizeof(unsigned short)),
		NULL,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			InitialVboSizeInSprites * QuadVertexElementCount *
			sizeof(GLfloat)),
		NULL,
		GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//	===========================================================================
void SpriteBatch::render(const glm::mat4& transform) {
	glUseProgram(mShader.id);
	glUniformMatrix4fv(mShader.mvpLocation, 1, GL_FALSE, &transform[0][0]);

	glBindVertexArray(mVao);

	//	Orphan buffer (or reallocate if too small)
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			mVertexBufferSizeInElements * sizeof(GLfloat)),
		NULL,
		GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			mIndexBufferSizeInElements * sizeof(unsigned short)),
		NULL,
		GL_STREAM_DRAW);

	this->renderBatches(mBatchByTexture, false);
	this->renderBatches(mAlphaBatchByTexture, true);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//	===========================================================================
void SpriteBatch::renderBatches(
	std::unordered_map<GLuint, Batch> batches, bool alpha) {
	if (alpha) {
		//	Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	for (auto b : batches) {
		Batch& batch = b.second;

		if (batch.QuadCount == 0) {
			continue;
		}

		//	Bind batch texture
		glBindTexture(GL_TEXTURE_2D, batch.TextureId);

		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			batch.VertexElementCount * sizeof(GLfloat),
			&batch.VertexData[0]);

		glBufferSubData(
			GL_ELEMENT_ARRAY_BUFFER,
			0,
			batch.IndexElementCount * sizeof(unsigned short),
			&batch.IndexData[0]);

		glDrawElements(
			GL_TRIANGLES,
			batch.IndexElementCount,
			GL_UNSIGNED_SHORT,
			(void*)0);
	}

	if (alpha) {
		//	Disable alpha blending
		glDisable(GL_BLEND);
	}
}
}