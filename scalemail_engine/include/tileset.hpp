#pragma once

#include "texture.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
class Tileset
{
public:
	Tileset() {
		tileWidth = 0;
		tileHeight = 0;
	}

	Tileset(Texture texture, int tileWidth, int tileHeight);
	void getTileUv(int index, glm::vec2& uv1, glm::vec2& uv2);
    Texture texture;
	int tileHeight;
	int tileWidth;
};

void getTilesetUv(const int index, const int width, const int height,
                  const int tileWidth, const int tileHeight,
                  glm::vec2& uv1, glm::vec2& uv2);
}