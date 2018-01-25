#pragma once

#include "gl_headers.hpp"
#include "fade_shader.hpp"
#include "quad_shader.hpp"
#include "mesh.hpp"
#include "sprite_shader.hpp"
#include "texture.hpp"
#include "tile_shader.hpp"
#include <map>
#include <string>

namespace ScaleMail
{
class AssetManager
{
    Mesh mQuadMesh;

    FadeShader mFadeShader;
    QuadShader mQuadShader;
    SpriteShader mSpriteShader;
    TileShader mTileShader;

    std::map<std::string, Texture> mTexturesByPath;
    std::map<int, Texture> mTexturesById;

public:
    FadeShader getFadeShader();
    Mesh getQuadMesh();
    QuadShader getQuadShader();
    SpriteShader getSpriteShader();
    TileShader getTileShader();
    Texture getTextureById(const int textureId);
    void initialize();
    Texture loadTexture(const std::string textureName);
};
}