#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "texture.hpp"
#include "tile_shader.hpp"
#include <glm/gtx/transform2.hpp>
#include <cmath>

namespace ScaleMail
{
static TileShader tileShader;

static Texture worldTexture;
static Texture horzScrollTexture;

static const float tileDuration = 1.0f;
static float tileTicks = 0;
static int tileFrame = 0;

//	============================================================================
void initializeMapMesh(AssetManager& assetManager) {
	worldTexture = assetManager.loadTexture("world");
	horzScrollTexture = assetManager.loadTexture("h_scroll");

	tileShader = assetManager.getTileShader();
}

//	============================================================================
void renderMap(const Map& map, const Camera& camera,
			   double totalElapsedSeconds) {
	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	glEnable(GL_DEPTH_TEST);

	glUseProgram(tileShader.id);
	glUniformMatrix4fv(tileShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);

	const auto& mapModel = map.getMapModel();
	const auto& mapMeshes = mapModel.getMapMeshes();

	for (const auto& mapMesh : mapMeshes) {
		const Mesh& mesh = mapMesh.mesh;

		if (mesh.vertexCount > 0) {
			if (mapMesh.animated &&
				mapMesh.frame != tileFrame) {
				continue;
			}

			if (mapMesh.alpha) {
				blendAlpha();
			} else {
				blendNone();
			}

			if (mapMesh.scroll) {
				glUniform1f(tileShader.timeLocation, totalElapsedSeconds * 0.5f);
			} else {
				glUniform1f(tileShader.timeLocation, 0);
			}

			glBindTexture(GL_TEXTURE_2D, mapMesh.textureId);

			drawMesh(mesh);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

//	============================================================================
void updateMapMeshAnimation(float elapsedSeconds) {
	tileTicks += elapsedSeconds;
	if (tileTicks >= tileDuration) {
		tileTicks -= tileDuration;
		++tileFrame;
		if (tileFrame > 1) {
			tileFrame = 0;
		}
	}
}
}