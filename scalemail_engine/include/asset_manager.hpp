#pragma once

#include "gl_headers.hpp"
#include "fade_shader.hpp"
#include "quad_shader.hpp"
#include "line_shader.hpp"
#include "mesh.hpp"
#include "shader_version.hpp"
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
	Texture mMissingTexture;

	Mesh mQuadMesh;

	QuadShader mColorQuadShader;
	QuadShader mParticleShader;
	FadeShader mFadeShader;
	QuadShader mQuadShader;
	LineShader mLineShader;
	SpriteShader mSpriteShader;
	TileShader mTileShader;

	std::string mShaderPath;

	std::map<std::string, Texture> mTexturesByPath;
	std::map<int, Texture> mTexturesById;

	void loadShader(GLuint& shaderId, const std::string& shaderName);

public:
	AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
	QuadShader getColorQuadShader();
	FadeShader getFadeShader();
	Mesh getQuadMesh();
	QuadShader getQuadShader();
	LineShader getLineShader();
	QuadShader getParticleShader();
	SpriteShader getSpriteShader();
	Tileset getTileset(const std::string textureName);
	TileShader getTileShader();
	Texture getTextureById(const int textureId);
	void initialize(const ShaderVersion shaderVersion);
	Texture loadTexture(const std::string textureName);
	Texture loadTexture(
		const std::string textureName,
		const std::string filename);
};
}