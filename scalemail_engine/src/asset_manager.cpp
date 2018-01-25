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
SpriteShader AssetManager::getSpriteShader() {
    return mSpriteShader;
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

    initShaderProgram("assets/shaders/sprite.vert", "assets/shaders/sprite.frag",
                      mSpriteShader.id);
    mSpriteShader.mvpLocation = glGetUniformLocation(mSpriteShader.id, "MVP");

    initShaderProgram("assets/shaders/tile.vert", "assets/shaders/tile.frag",
                      mTileShader.id);
    mTileShader.mvpLocation = glGetUniformLocation(mTileShader.id, "MVP");
    mTileShader.timeLocation = glGetUniformLocation(mTileShader.id, "time");
}

//  ============================================================================
Texture AssetManager::loadTexture(const std::string textureName) {
    std::string texturePath = getTexturePath(textureName);

    auto result = mTexturesByPath.find(texturePath);

    if (result != mTexturesByPath.end()) {
        std::cout << textureName << " texture found in cache." << std::endl;
        return result->second;
    }

    Texture texture;
    loadPngTexture(texturePath, texture);

    mTexturesByPath.emplace(texturePath, texture);
    mTexturesById.emplace(texture.id, texture);

    std::cout << textureName << " texture loaded." << std::endl;

    return texture;
}
}