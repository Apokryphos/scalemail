#pragma once

#include "texture.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace ScaleMail
{
class Tileset
{
	int mTileHeight;
	int mTileWidth;
	Texture mTexture;

public:
	Tileset();
	Tileset(Texture texture, int tileWidth, int tileHeight);
	const Texture& getTexture() const;
	int getTileHeight() const;
	void getTileUv(const int index, glm::vec2& uv1, glm::vec2& uv2) const;
	void getTileUv(const int index, const glm::ivec4& sourceRect,
				   glm::vec2& uv1, glm::vec2& uv2) const;
	int getTileWidth() const;
};
}