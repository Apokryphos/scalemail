#include "asset_manager.hpp"
#include "blend.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "mesh.hpp"
#include "quad_shader.hpp"
#include "texture.hpp"
#include "tiny_font.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace ScaleMail
{
struct TextData {
    glm::vec2 position;
    glm::vec4 color;
    float size;
    std::string text;
};

size_t fontVertexBufferSize = 0;

static const glm::vec2 fontSize = glm::vec2(0.857142857f, 1);

static Texture fontTexture;
static std::vector<TextData> drawTextData;
static std::vector<float> fontVertexData;

static Mesh mesh;

static QuadShader quadShader;

//	============================================================================
glm::vec2 measureText(const std::string& text, const float size) {
    return glm::vec2(text.length(), 0) * fontSize * size;
}

//	============================================================================
void drawText(const glm::vec2 position, const std::string& text,
              const glm::vec4 color, const float size) {

    std::string str = text;
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        ::toupper);

    drawTextData.push_back({ position, color, size, str });
}

//	============================================================================
void drawCenterText(const glm::vec2 position, const std::string& text,
                    const glm::vec4 color, const float size) {

    glm::vec2 textSize = measureText(text, size);
    drawText(
        position - textSize * glm::vec2(0.5f, 0),
        text,
        color,
        size);
}

//  ============================================================================
static bool initFontMesh(Mesh& mesh) {

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    mesh.vertexCount = 6;

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof([]), [],
    //              GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) (sizeof(float) * 2));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 8, (void*) (sizeof(float) * 6));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

//	============================================================================
void initializeFont(AssetManager& assetManager) {
    initFontMesh(mesh);

    fontTexture = assetManager.loadTexture("font");

    quadShader = assetManager.getQuadShader();
}

//	============================================================================
void renderText(const GameWindow& gameWindow) {
    glEnable(GL_BLEND);
    blendAlpha();

    int glyphCount = 0;

    for (const auto& textData : drawTextData) {
        glm::vec2 position = textData.position;
        glm::vec2 size = fontSize * textData.size;

        for (const char& c : textData.text) {
            int index = toTinyDungeonFontIndex((int)c);

            if (index < 0) {
                position.x += size.x;
                continue;
            }

            ++glyphCount;

            int x = index % 36;
            int y = index / 36;

            float xStep = 6 / 216.0f;
            float yStep = 7 / 42.0f;

            float u1 = x * xStep;
            float v1 = y * yStep;

            float u2 = u1 + xStep;
            float v2 = v1 + yStep;

            fontVertexData.push_back(position.x);
            fontVertexData.push_back(position.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u1);
            fontVertexData.push_back(v1);

            fontVertexData.push_back(position.x);
            fontVertexData.push_back(position.y + size.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u1);
            fontVertexData.push_back(v2);

            fontVertexData.push_back(position.x + size.x);
            fontVertexData.push_back(position.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u2);
            fontVertexData.push_back(v1);

            fontVertexData.push_back(position.x);
            fontVertexData.push_back(position.y + size.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u1);
            fontVertexData.push_back(v2);

            fontVertexData.push_back(position.x + size.x);
            fontVertexData.push_back(position.y + size.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u2);
            fontVertexData.push_back(v2);

            fontVertexData.push_back(position.x + size.x);
            fontVertexData.push_back(position.y);
            fontVertexData.push_back(textData.color.r);
            fontVertexData.push_back(textData.color.g);
            fontVertexData.push_back(textData.color.b);
            fontVertexData.push_back(textData.color.a);
            fontVertexData.push_back(u2);
            fontVertexData.push_back(v1);

            position.x += size.x;
        }
    }

    mesh.vertexCount = glyphCount * 6;

    glm::mat4 projection =
        glm::ortho(0.0f, (float)gameWindow.width, (float)gameWindow.height, 0.0f,
                   0.0f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    size_t vertexDataSize = sizeof(float) * fontVertexData.size();

    if (fontVertexBufferSize < vertexDataSize) {
        fontVertexBufferSize = vertexDataSize;

        glBufferData(GL_ARRAY_BUFFER, fontVertexBufferSize,
                     &fontVertexData[0], GL_STATIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, fontVertexBufferSize,
                        &fontVertexData[0]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, gameWindow.width, gameWindow.height);

    glUseProgram(quadShader.id);
    glUniformMatrix4fv(quadShader.mvpLocation, 1, GL_FALSE, &projection[0][0]);

    glBindTexture(GL_TEXTURE_2D, fontTexture.id);
    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
    glBindVertexArray(0);

    drawTextData.clear();
    fontVertexData.clear();
}
}