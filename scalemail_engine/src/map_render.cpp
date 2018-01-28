#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "texture.hpp"
#include "tile_shader.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
void renderMap(GameWindow& gameWindow, const Map& map, const Camera& camera,
               float totalElapsedSeconds) {
    //  Draw map
    const Mesh& mesh = map.mapMesh.staticMesh;

    glm::mat4 projection =
        glm::ortho(0.0f, (float)gameWindow.width,
                   (float)gameWindow.height, 0.0f,
                   0.0f, 1.0f);

    glm::mat4 mvp = projection * camera.getView();

    blendAlpha();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(tileShader.id);
    glUniform1f(tileShader.timeLocation, 0);
    glUniformMatrix4fv(tileShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);
    glBindTexture(GL_TEXTURE_2D, worldTexture.id);
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

    const Mesh& animMesh = map.mapMesh.scrollMeshes[tileFrame];
    glBindVertexArray(animMesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, animMesh.vertexCount);

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