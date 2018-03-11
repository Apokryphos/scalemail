#include "asset_manager.hpp"
#include "render_options.hpp"
#include "shader.hpp"
#include <iostream>

namespace ScaleMail
{
static const std::vector<float> QUAD_VERTEX_DATA = {
	 1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 0.0f,
	 1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f, 1.0f,
	-1.0f,  1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 1.0f,
	-1.0f, -1.0f, 1.f, 1.f, 1.f, 1.f, 0.0f, 0.0f
};

//  ============================================================================
static std::string getTexturePath(std::string textureName) {
	return "assets/textures/" + textureName + ".png";
}

//  ============================================================================
QuadShader AssetManager::getColorQuadShader() {
	return mColorQuadShader;
}

//  ============================================================================
FadeShader AssetManager::getFadeShader() {
	return mFadeShader;
}

//  ============================================================================
Mesh AssetManager::getQuadMesh() {
	return mQuadMesh;
}

//  ============================================================================
QuadShader AssetManager::getQuadShader() {
	return mQuadShader;
}

//  ============================================================================
LineShader AssetManager::getLineShader() {
	return mLineShader;
}

//  ============================================================================
QuadShader AssetManager::getParticleShader() {
	return mParticleShader;
}

//  ============================================================================
SpriteShader AssetManager::getSpriteShader() {
	return mSpriteShader;
}

//  ============================================================================
Tileset AssetManager::getTileset(const std::string textureName) {
	Texture texture = this->loadTexture(textureName);
	return Tileset(texture, 16, 16);
}

//  ============================================================================
TileShader AssetManager::getTileShader() {
	return mTileShader;
}

//  ============================================================================
Texture AssetManager::getTextureById(const int textureId) {
	auto result = mTexturesById.find(textureId);

	if (result != mTexturesById.end()) {
		return result->second;
	}

	std::cerr << "AssetManager::getTextureById: Texture ID not found." << std::endl;
	throw std::runtime_error("Texture not found.");
}

//  ============================================================================
void AssetManager::initialize(const RenderOptions& renderOptions) {
	switch (renderOptions.shaderVersion) {
		case ShaderVersion::SHADER_VERSION_120:
			mShaderPath = "assets/shaders/120/";
			break;

		case ShaderVersion::SHADER_VERSION_330:
			mShaderPath = "assets/shaders/330/";
			break;

		default:
			throw std::runtime_error("Case not implemented.");
	}

	mMissingTexture = this->loadTexture("checker");

	this->loadTexture("dirt_mask", "dirt");
	this->loadTexture("fx_mask", "fx");
	this->loadTexture("world_mask", "world");

	initMesh(mQuadMesh, VertexDefinition::POSITION2_COLOR4_TEXTURE2,
			 renderOptions);
	setMeshVertexData(mQuadMesh, QUAD_VERTEX_DATA);

	this->loadShader(mColorQuadShader.id, "color");
	mColorQuadShader.mvpLocation =
		glGetUniformLocation(mColorQuadShader.id, "MVP");

	this->loadShader(mFadeShader.id, "fade");
	mFadeShader.fadeColorLocation =
		glGetUniformLocation(mFadeShader.id, "fadeColor");
	mFadeShader.fadeProgressLocation =
		glGetUniformLocation(mFadeShader.id, "fadeProgress");

	this->loadShader(mQuadShader.id, "flat");
	mQuadShader.mvpLocation = glGetUniformLocation(mQuadShader.id, "MVP");

	this->loadShader(mLineShader.id, "line");
	mLineShader.mvpLocation = glGetUniformLocation(mLineShader.id, "MVP");

	this->loadShader(mParticleShader.id, "particle");
	mParticleShader.mvpLocation =
		glGetUniformLocation(mParticleShader.id, "MVP");

	this->loadShader(mSpriteShader.id, "sprite");
	mSpriteShader.mvpLocation = glGetUniformLocation(mSpriteShader.id, "MVP");
	mSpriteShader.alphaLocation =
		glGetUniformLocation(mSpriteShader.id, "alpha");

	this->loadShader(mTileShader.id, "tile");
	mTileShader.mvpLocation = glGetUniformLocation(mTileShader.id, "MVP");
	mTileShader.timeLocation = glGetUniformLocation(mTileShader.id, "time");
}

//  ============================================================================
void AssetManager::loadShader(GLuint& shaderId, const std::string& shaderName) {
	initShaderProgram(
		mShaderPath + shaderName + ".vert",
		mShaderPath + shaderName + ".frag",
		shaderId);
}

//  ============================================================================
Texture AssetManager::loadTexture(const std::string textureName) {
	return this->loadTexture(textureName, textureName);
}

//  ============================================================================
Texture AssetManager::loadTexture(
	const std::string textureName,
	const std::string filename) {
	std::string texturePath = getTexturePath(textureName);

	auto result = mTexturesByPath.find(texturePath);

	if (result != mTexturesByPath.end()) {
		return result->second;
	}

	Texture texture;
	std::string filePath = getTexturePath(filename);
	if (!loadPngTexture(filePath, texture)) {
		texture = mMissingTexture;
		std::cout << textureName << " texture missing." << std::endl;
	}

	mTexturesByPath.emplace(texturePath, texture);
	mTexturesById.emplace(texture.id, texture);

	std::cout << textureName << " texture loaded." << std::endl;

	return texture;
}
}