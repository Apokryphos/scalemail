#pragma once

#include "gl_headers.hpp"
#include "fade_shader.hpp"
#include "quad_shader.hpp"
#include "line_shader.hpp"
#include "mesh.hpp"
#include "sprite_shader.hpp"
#include "texture.hpp"
#include "tile_shader.hpp"
#include "tileset.hpp"
#include <map>
#include <string>

namespace ScaleMail
{
class AssetManager
{
	Mesh mQuadMesh;

	QuadShader mColorQuadShader;
	FadeShader mFadeShader;
	QuadShader mQuadShader;
	LineShader mLineShader;
	SpriteShader mSpriteShader;
	TileShader mTileShader;

	std::map<std::string, Texture> mTexturesByPath;
	std::map<int, Texture> mTexturesById;

public:
	QuadShader getColorQuadShader();
	FadeShader getFadeShader();
	Mesh getQuadMesh();
	QuadShader getQuadShader();
	LineShader getLineShader();
	SpriteShader getSpriteShader();
	Tileset getTileset(const std::string textureName);
	TileShader getTileShader();
	Texture getTextureById(const int textureId);
	void initialize();
	Texture loadTexture(const std::string textureName);
};
}