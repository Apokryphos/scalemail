#include "tileset.hpp"

namespace ScaleMail
{
//  ============================================================================
Tileset::Tileset() : mTileHeight(0), mTileWidth(0) {
}

//  ============================================================================
Tileset::Tileset(Texture texture, int tileWidth, int tileHeight)
	: mTileHeight(tileHeight), mTileWidth(tileWidth), mTexture(texture) {
}

//  ============================================================================
const Texture& Tileset::getTexture() const {
	return mTexture;
}

//  ============================================================================
int Tileset::getTileHeight() const {
	return mTileHeight;
}

//  ============================================================================
void Tileset::getTileUv(const int index, glm::vec2& uv1, glm::vec2& uv2) const {
	const int TILES_PER_ROW = mTexture.width / mTileWidth;

	int x = index % TILES_PER_ROW;
	int y = index / TILES_PER_ROW;

	int pixelX = x * mTileWidth;
	int pixelY = y * mTileHeight;

	double u1 = pixelX / (double)mTexture.width;
	double v1 = pixelY / (double)mTexture.height;

	double u2 = (pixelX + mTileWidth) / (double)mTexture.width;
	double v2 = (pixelY + mTileHeight) / (double)mTexture.height;

	uv1 = glm::vec2(u1, v2);
	uv2 = glm::vec2(u2, v1);
}

//  ============================================================================
void Tileset::getTileUv(const int index, const glm::ivec4& sourceRect,
						glm::vec2& uv1, glm::vec2& uv2) const {
	const int TILES_PER_ROW = mTexture.width / mTileWidth;

	int x = index % TILES_PER_ROW;
	int y = index / TILES_PER_ROW;

	int pixelX = x * mTileWidth;
	int pixelY = y * mTileHeight;

	double u1 = (pixelX + sourceRect.x) / (double)mTexture.width;
	double v1 = (pixelY + sourceRect.y) / (double)mTexture.height;

	double u2 = (pixelX + sourceRect[2]) / (double)mTexture.width;
	double v2 = (pixelY + sourceRect[3]) / (double)mTexture.height;

	uv1 = glm::vec2(u1, v2);
	uv2 = glm::vec2(u2, v1);
}

//  ============================================================================
int Tileset::getTileWidth() const {
	return mTileWidth;
}
}