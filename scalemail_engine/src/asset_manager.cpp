#include "asset_manager.hpp"
#include "shader.hpp"
#include <iostream>

namespace ScaleMail
{
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
void AssetManager::initialize() {
	mMissingTexture = this->loadTexture("checker");

	this->loadTexture("dirt_mask", "dirt");
	this->loadTexture("fx_mask", "fx");
	this->loadTexture("world_mask", "world");

	initQuadMesh(mQuadMesh);

	initShaderProgram("assets/shaders/fade.vert", "assets/shaders/fade.frag",
					  mFadeShader.id);
	mFadeShader.fadeColorLocation =
		glGetUniformLocation(mFadeShader.id, "fadeColor");
	mFadeShader.fadeProgressLocation =
		glGetUniformLocation(mFadeShader.id, "fadeProgress");

	initShaderProgram("assets/shaders/flat.vert", "assets/shaders/flat.frag",
					  mQuadShader.id);
	mQuadShader.mvpLocation = glGetUniformLocation(mQuadShader.id, "MVP");

	initShaderProgram("assets/shaders/color.vert", "assets/shaders/color.frag",
					  mColorQuadShader.id);
	mColorQuadShader.mvpLocation = glGetUniformLocation(mColorQuadShader.id, "MVP");

	initShaderProgram("assets/shaders/line.vert", "assets/shaders/line.frag",
					  mLineShader.id);
	mLineShader.mvpLocation = glGetUniformLocation(mLineShader.id, "MVP");

	initShaderProgram("assets/shaders/particle.vert", "assets/shaders/particle.frag",
					  mParticleShader.id);
	mParticleShader.mvpLocation = glGetUniformLocation(mParticleShader.id, "MVP");

	initShaderProgram("assets/shaders/sprite.vert", "assets/shaders/sprite.frag",
					  mSpriteShader.id);
	mSpriteShader.mvpLocation = glGetUniformLocation(mSpriteShader.id, "MVP");
	mSpriteShader.alphaLocation = glGetUniformLocation(mSpriteShader.id, "alpha");

	initShaderProgram("assets/shaders/tile.vert", "assets/shaders/tile.frag",
					  mTileShader.id);
	mTileShader.mvpLocation = glGetUniformLocation(mTileShader.id, "MVP");
	mTileShader.timeLocation = glGetUniformLocation(mTileShader.id, "time");
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