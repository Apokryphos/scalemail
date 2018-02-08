#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "texture.hpp"
#include "tile_shader.hpp"
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
static TileShader tileShader;

static Texture worldTexture;
static Texture horzScrollTexture;

static const float tileDuration = 0.33f;
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
			   float totalElapsedSeconds) {
	//  Draw map
	const Mesh& mesh = map.mapMesh.staticMesh;

	glm::mat4 mvp = camera.getProjection() * camera.getView();

	glEnable(GL_DEPTH_TEST);
	blendAlpha();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(tileShader.id);
	glUniform1f(tileShader.timeLocation, 0);
	glUniformMatrix4fv(tileShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glBindTexture(GL_TEXTURE_2D, worldTexture.id);
	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

	glDisable(GL_DEPTH_TEST);
	const Mesh& alphaMesh = map.mapMesh.alphaMesh;
	glBindVertexArray(alphaMesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, alphaMesh.vertexCount);

	glEnable(GL_DEPTH_TEST);
	const Mesh& animMesh = map.mapMesh.animatedMeshes[tileFrame];
	if (animMesh.vertexCount > 0) {
		glBindVertexArray(animMesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, animMesh.vertexCount);
	}

	const Mesh& scrollMesh = map.mapMesh.scrollMeshes[tileFrame];
	glUniform1f(tileShader.timeLocation, totalElapsedSeconds * 0.5f);
	glBindTexture(GL_TEXTURE_2D, horzScrollTexture.id);
	glBindVertexArray(scrollMesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, scrollMesh.vertexCount);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//	============================================================================
void updateMapMesh(float elapsedSeconds) {
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