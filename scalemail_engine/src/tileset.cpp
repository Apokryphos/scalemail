#include "tileset.hpp"

namespace ScaleMail
{
//  ============================================================================
Tileset::Tileset(Texture texture, int tileWidth, int tileHeight)
	: tileHeight(tileHeight), tileWidth(tileWidth) {
	this->texture = texture;
}

//  ============================================================================
void Tileset::getTileUv(int index, glm::vec2& uv1, glm::vec2& uv2) {
	const int TILES_PER_ROW = this->texture.width / this->tileWidth;

	int x = index % TILES_PER_ROW;
	int y = index / TILES_PER_ROW;

	int pixelX = x * this->tileWidth;
	int pixelY = y * this->tileHeight;

	double u1 = pixelX / (double)this->texture.width;
	double v1 = pixelY / (double)this->texture.height;

	double u2 = (pixelX + this->tileWidth) / (double)this->texture.width;
	double v2 = (pixelY + this->tileHeight) / (double)this->texture.height;

	uv1 = glm::vec2(u1, v1);
	uv2 = glm::vec2(u2, v2);
}


//  ============================================================================
void getTilesetUv(const int index, const int width, const int height,
				  const int tileWidth, const int tileHeight,
				  glm::vec2& uv1, glm::vec2& uv2) {
	const int TILES_PER_ROW = width / tileWidth;

	int x = index % TILES_PER_ROW;
	int y = index / TILES_PER_ROW;

	int pixelX = x * tileWidth;
	int pixelY = y * tileHeight;

	double u1 = pixelX / (double)width;
	double v1 = pixelY / (double)height;

	double u2 = (pixelX + tileWidth) / (double)width;
	double v2 = (pixelY + tileHeight) / (double)height;

	uv1 = glm::vec2(u1, v1);
	uv2 = glm::vec2(u2, v2);
}
}