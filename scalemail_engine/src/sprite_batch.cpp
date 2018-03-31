#include "asset_manager.hpp"
#include "gl_headers.hpp"
#include "math_util.hpp"
#include "sprite_batch.hpp"
#include "texture.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

namespace ScaleMail
{
static const glm::vec3 gSpriteVertices[] = {
	glm::vec3(-0.5f, -0.5f, 1.0f),
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f, -0.5f, 1.0f),
};

static const glm::vec2 gQuadVertices[] = {
	glm::vec2(-0.5f, -0.5f),
	glm::vec2(-0.5f,  0.5f),
	glm::vec2( 0.5f,  0.5f),
	glm::vec2( 0.5f, -0.5f),
};

static const glm::vec2 gTileVertices[] = {
	glm::vec2( 0.0f, 0.0f),
	glm::vec2( 0.0f, 1.0f),
	glm::vec2( 1.0f, 1.0f),
	glm::vec2( 1.0f, 0.0f),
};

//	===========================================================================
static void disableVertexAttributes() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

//	===========================================================================
static void enableVertexAttributes(const GLsizei stride) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
}

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

	batch.quadCount += quadCount;

	//	Assign texture ID in case batch was created
	batch.textureId = textureId;

	this->resizeBatchData(batch, quadCount);

	//	Build vertex data
	for (int s = 0; s < quadCount; s++) {
		Batch& batch = alpha ? mAlphaBatchByTexture.at(textureId)
							 : mBatchByTexture.at(textureId);

		std::vector<float>&          vertexData = batch.vertexData;
		std::vector<unsigned short>& indexData  = batch.indexData;

		unsigned short& vertex = batch.vertexOffset;
		size_t&         e      = batch.indexOffset;
		size_t&         v      = batch.vertexElementOffset;

		glm::vec2 quadA = gQuadVertices[0];
		glm::vec2 quadB = gQuadVertices[1];
		glm::vec2 quadC = gQuadVertices[2];
		glm::vec2 quadD = gQuadVertices[3];

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadA.x;
		vertexData[v++] = position[s].y + size[s].y * quadA.y;
		vertexData[v++] = position[s].z;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;
		//	UV
		vertexData[v++] = 0.0f;
		vertexData[v++] = 1.0f;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadB.x;
		vertexData[v++] = position[s].y + size[s].y * quadB.y;
		vertexData[v++] = position[s].z;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;
		//	UV
		vertexData[v++] = 0.0f;
		vertexData[v++] = 0.0f;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadC.x;
		vertexData[v++] = position[s].y + size[s].y * quadC.y;
		vertexData[v++] = position[s].z;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;
		//	UV
		vertexData[v++] = 1.0f;
		vertexData[v++] = 0.0f;

		//	Position
		vertexData[v++] = position[s].x + size[s].x * quadD.x;
		vertexData[v++] = position[s].y + size[s].y * quadD.y;
		vertexData[v++] = position[s].z;
		//	Color
		vertexData[v++] = color[s].r;
		vertexData[v++] = color[s].g;
		vertexData[v++] = color[s].b;
		vertexData[v++] = color[s].a;
		//	UV
		vertexData[v++] = 1.0f;
		vertexData[v++] = 1.0f;

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
	glm::vec2 uv1;
	glm::vec2 uv2;
	tileset.getTileUv(tilesetId, uv1, uv2);

	this->buildTileVertexData(
		tileset.getTexture().id, position, size, uv1, uv2, alpha);
}

//	===========================================================================
void SpriteBatch::buildTileVertexData(
	int textureId, const glm::vec2& position, const glm::vec2& size,
	const glm::vec2& uv1, const glm::vec2& uv2,	bool alpha) {
	Batch& batch = alpha ? mAlphaBatchByTexture[textureId]
						 : mBatchByTexture[textureId];
	++batch.quadCount;

	//	Assign texture ID in case batch was created
	batch.textureId = textureId;

	this->resizeBatchData(batch, 1);

	std::vector<float>&          vertexData = batch.vertexData;
	std::vector<unsigned short>& indexData  = batch.indexData;

	unsigned short& vertex = batch.vertexOffset;
	size_t&         e      = batch.indexOffset;
	size_t&         v      = batch.vertexElementOffset;

	glm::vec2 quadA = gTileVertices[0];
	glm::vec2 quadB = gTileVertices[1];
	glm::vec2 quadC = gTileVertices[2];
	glm::vec2 quadD = gTileVertices[3];

	//	Position
	vertexData[v++] = position.x + size.x * quadA.x;
	vertexData[v++] = position.y + size.y * quadA.y;
	vertexData[v++] = 0.0f;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	//	UV
	vertexData[v++] = uv1.x;
	vertexData[v++] = uv2.y;

	//	Position
	vertexData[v++] = position.x + size.x * quadB.x;
	vertexData[v++] = position.y + size.y * quadB.y;
	vertexData[v++] = 0.0f;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	//	UV
	vertexData[v++] = uv1.x;
	vertexData[v++] = uv1.y;

	//	Position
	vertexData[v++] = position.x + size.x * quadC.x;
	vertexData[v++] = position.y + size.y * quadC.y;
	vertexData[v++] = 0.0f;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	//	UV
	vertexData[v++] = uv2.x;
	vertexData[v++] = uv1.y;

	//	Position
	vertexData[v++] = position.x + size.x * quadD.x;
	vertexData[v++] = position.y + size.y * quadD.y;
	vertexData[v++] = 0.0f;
	//	Color
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	vertexData[v++] = 1.0f;
	//	UV
	vertexData[v++] = uv2.x;
	vertexData[v++] = uv2.y;

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
	for (const auto& boolMap : mTextureIdCounts) {
		bool alpha = boolMap.first;

		for (const auto& p : boolMap.second) {
			Batch& batch = alpha ? mAlphaBatchByTexture[p.first]
								 : mBatchByTexture[p.first];

			batch.quadCount += p.second;

			//	Assign texture ID in case batch was created
			batch.textureId = p.first;

			this->resizeBatchData(batch, p.second);
		}
	}

	//	Build vertex data
	for (int s = 0; s < spriteCount; s++) {
		Batch& batch = alpha[s] ? mAlphaBatchByTexture.at(textureId[s])
								: mBatchByTexture.at(textureId[s]);

		std::vector<float>&          vertexData = batch.vertexData;
		std::vector<unsigned short>& indexData  = batch.indexData;

		unsigned short& vertex = batch.vertexOffset;
		size_t&         e      = batch.indexOffset;
		size_t&         v      = batch.vertexElementOffset;

		glm::mat3 rotation =
			glm::rotate(rotate[s], glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 quadA = rotation * gSpriteVertices[0];
		glm::vec3 quadB = rotation * gSpriteVertices[1];
		glm::vec3 quadC = rotation * gSpriteVertices[2];
		glm::vec3 quadD = rotation * gSpriteVertices[3];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadA.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadA.y;
		vertexData[v++] = positionZ[s] * sizeY[s] * quadA.z;
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];
		//	UV
		vertexData[v++] = texU1[s];
		vertexData[v++] = texV2[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadB.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadB.y;
		vertexData[v++] = positionZ[s] * sizeY[s] * quadB.z;
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];
		//	UV
		vertexData[v++] = texU1[s];
		vertexData[v++] = texV1[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadC.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadC.y;
		vertexData[v++] = positionZ[s] * sizeY[s] * quadC.z;
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];
		//	UV
		vertexData[v++] = texU2[s];
		vertexData[v++] = texV1[s];

		//	Position
		vertexData[v++] = positionX[s] + sizeX[s] * quadD.x;
		vertexData[v++] = positionY[s] + sizeY[s] * quadD.y;
		vertexData[v++] = positionZ[s] * sizeY[s] * quadD.z;
		//	Color
		vertexData[v++] = colorR[s];
		vertexData[v++] = colorG[s];
		vertexData[v++] = colorB[s];
		vertexData[v++] = colorA[s];
		//	UV
		vertexData[v++] = texU2[s];
		vertexData[v++] = texV2[s];

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
void SpriteBatch::initialize(const std::string& spriteBatchName,
							 AssetManager& assetManager) {
	mName = spriteBatchName;

	const GLsizei stride =
		static_cast<GLsizei>(VertexElementCount * sizeof(GLfloat));

	mShader = assetManager.getSpriteShader();

	const bool vaoSupported = assetManager.getRenderCaps().vaoSupported;

	if (vaoSupported) {
		glGenVertexArrays(1, &mVao);
	} else {
		mVao = 0;
	}

	glGenBuffers(1, &mVertexBuffer);
	glGenBuffers(1, &mIndexBuffer);

	if (vaoSupported) {
		glBindVertexArray(mVao);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			InitialSpriteCount * QuadIndexCount *
			sizeof(unsigned short)),
		NULL,
		GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(
			InitialSpriteCount * QuadVertexElementCount *
			sizeof(GLfloat)),
		NULL,
		GL_STREAM_DRAW);

	enableVertexAttributes(stride);

	if (vaoSupported) {
		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	disableVertexAttributes();
}

//	===========================================================================
void SpriteBatch::render(const glm::mat4& transform) {
	glUseProgram(mShader.id);
	glUniformMatrix4fv(mShader.mvpLocation, 1, GL_FALSE, &transform[0][0]);
	glUniform1f(mShader.alphaLocation, mAlpha);

	const GLsizei stride =
		static_cast<GLsizei>(VertexElementCount * sizeof(GLfloat));

	if (mVao != 0) {
		glBindVertexArray(mVao);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		enableVertexAttributes(stride);
	}

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

	if (mVao != 0) {
		glBindVertexArray(0);
	} else {
		disableVertexAttributes();
	}

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

	for (const auto& b : batches) {
		const Batch& batch = b.second;

		if (batch.quadCount == 0) {
			continue;
		}

		//	Bind batch texture
		glBindTexture(GL_TEXTURE_2D, batch.textureId);

		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			batch.vertexElementCount * sizeof(GLfloat),
			&batch.vertexData[0]);

		glBufferSubData(
			GL_ELEMENT_ARRAY_BUFFER,
			0,
			batch.indexElementCount * sizeof(unsigned short),
			&batch.indexData[0]);

		glDrawElements(
			GL_TRIANGLES,
			batch.indexElementCount,
			GL_UNSIGNED_SHORT,
			(void*)0);
	}

	if (alpha) {
		//	Disable alpha blending
		glDisable(GL_BLEND);
	}
}

//	===========================================================================
void SpriteBatch::resizeBatchData(Batch& batch, size_t quadCount) {
	//	Increase vertex buffer to size of largest batch
	batch.vertexElementCount += quadCount * QuadVertexElementCount;
	if (batch.vertexElementCount > mVertexBufferSizeInElements) {
		mVertexBufferSizeInElements = batch.vertexElementCount;
	}

	//	Increase index buffer to size of largest batch
	batch.indexElementCount += quadCount * QuadIndexCount;
	if (batch.indexElementCount > mIndexBufferSizeInElements) {
		mIndexBufferSizeInElements = batch.indexElementCount;
	}

	const size_t vertexDataSize = batch.vertexData.size();
	if (batch.vertexElementCount > vertexDataSize) {
		//	Only display message if resize will reallocate
		if (vertexDataSize > batch.vertexData.capacity()) {
			std::cout
				<< "SpriteBatch '" << mName
				<< "' allocated vertex data." << std::endl;
		}

		batch.vertexData.resize(batch.vertexElementCount * 2);
	}

	const size_t indexDataSize = batch.indexData.size();
	if (batch.indexElementCount > indexDataSize) {
		//	Only display message if resize will reallocate
		if (vertexDataSize > batch.vertexData.capacity()) {
			std::cout
				<< "SpriteBatch '" << mName
				<< "' allocated index data." << std::endl;
		}

		batch.indexData.resize(batch.indexElementCount * 2);
	}
}

//	===========================================================================
void SpriteBatch::setAlpha(float alpha) {
	mAlpha = clamp(alpha, 0.0f, 1.0f);
}
}