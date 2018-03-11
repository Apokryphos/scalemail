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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(tileShader.id);
	glUniform1f(tileShader.timeLocation, 0);
	glUniformMatrix4fv(tileShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindTexture(GL_TEXTURE_2D, worldTexture.id);

	const Mesh& mesh = map.mapMesh.staticMesh;
	if (mesh.vertexCount > 0) {
		blendNone();
		drawMesh(mesh);
	}

	const Mesh& alphaMesh = map.mapMesh.alphaMesh;
	if (alphaMesh.vertexCount > 0) {
		blendAlpha();
		drawMesh(alphaMesh);
	}

	glEnable(GL_DEPTH_TEST);
	const Mesh& animMesh = map.mapMesh.animatedMeshes[tileFrame];
	if (animMesh.vertexCount > 0) {
		blendAlpha();
		drawMesh(animMesh);
	}

	const Mesh& scrollMesh = map.mapMesh.scrollMeshes[tileFrame];
	if (scrollMesh.vertexCount > 0) {
		blendNone();
		glUniform1f(tileShader.timeLocation, totalElapsedSeconds * 0.5f);
		glBindTexture(GL_TEXTURE_2D, horzScrollTexture.id);
		drawMesh(scrollMesh);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	disableMeshVertexAttribPointers(mesh);
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