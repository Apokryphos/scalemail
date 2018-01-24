#include "gl_headers.hpp"
#include "map.hpp"
#include "mesh.hpp"
#include "tileset.hpp"
#include "TmxMapLib/Map.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ScaleMail
{
struct TileData {
    bool animated;
    int x;
    int y;
    int tilesetId;
    int nextFrame;
};

struct TileLayerData {
    std::vector<TileData> tiles;
};

struct MapData {
    std::vector<TileLayerData> tileLayers;
};

//  ============================================================================
static void createTileMeshBuffer(Mesh& mesh, std::vector<float>& meshVertexData) {
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    mesh.vertexCount = meshVertexData.size() / 4;

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    size_t vertexDataSize = sizeof(float) * meshVertexData.size();

    glBufferData(GL_ARRAY_BUFFER, vertexDataSize,
                 &meshVertexData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) (sizeof(float) * 2));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//  ============================================================================
static void addTileVertexData(std::vector<float>& meshVertexData,
                              const glm::vec2 position, const glm::vec2 size,
                              const glm::vec2 uv1, const glm::vec2 uv2) {
    float u1 = uv1.x;
    float v1 = uv1.y;
    float u2 = uv2.x;
    float v2 = uv2.y;

    meshVertexData.push_back(position.x);
    meshVertexData.push_back(position.y);
    meshVertexData.push_back(u1);
    meshVertexData.push_back(v1);

    meshVertexData.push_back(position.x);
    meshVertexData.push_back(position.y + size.y);
    meshVertexData.push_back(u1);
    meshVertexData.push_back(v2);

    meshVertexData.push_back(position.x + size.x);
    meshVertexData.push_back(position.y);
    meshVertexData.push_back(u2);
    meshVertexData.push_back(v1);

    meshVertexData.push_back(position.x);
    meshVertexData.push_back(position.y + size.y);
    meshVertexData.push_back(u1);
    meshVertexData.push_back(v2);

    meshVertexData.push_back(position.x + size.x);
    meshVertexData.push_back(position.y + size.y);
    meshVertexData.push_back(u2);
    meshVertexData.push_back(v2);

    meshVertexData.push_back(position.x + size.x);
    meshVertexData.push_back(position.y);
    meshVertexData.push_back(u2);
    meshVertexData.push_back(v1);
}

//  ============================================================================
static void buildMapMesh(const MapData& mapData, MapMesh& mapMesh) {
    std::vector<float> staticVertexData;
    std::vector<float> frame1VertexData;
    std::vector<float> frame2VertexData;
    std::vector<float> scrollFrame1VertexData;
    std::vector<float> scrollFrame2VertexData;

    for (const auto& tileLayer : mapData.tileLayers) {
        for (const auto& tile : tileLayer.tiles) {
            glm::vec2 size = glm::vec2(16);

            glm::vec2 position = glm::vec2(tile.x, tile.y) * size;

            int tilesetId = tile.tilesetId;

            glm::vec2 uv1, uv2;
            getTilesetUv(tilesetId, 256, 304, 16, 16, uv1, uv2);

            if (!tile.animated) {
                addTileVertexData(staticVertexData, position, size, uv1, uv2);
            } else {
                if (tilesetId >= 140 && tilesetId <= 143) {
                    getTilesetUv(tilesetId - 140, 32, 32, 16, 16, uv1, uv2);
                    addTileVertexData(scrollFrame1VertexData, position, size, uv1, uv2);
                    getTilesetUv(tile.nextFrame - 140, 32, 32, 16, 16, uv1, uv2);
                    addTileVertexData(scrollFrame2VertexData, position, size, uv1, uv2);
                } else {
                    addTileVertexData(frame1VertexData, position, size, uv1, uv2);
                    getTilesetUv(tile.nextFrame, 256, 304, 16, 16, uv1, uv2);
                    addTileVertexData(frame2VertexData, position, size, uv1, uv2);
                }
            }
        }
    }

    Mesh staticMesh;
    Mesh frame1Mesh;
    Mesh frame2Mesh;
    Mesh scrollFrame1Mesh;
    Mesh scrollFrame2Mesh;

    createTileMeshBuffer(staticMesh, staticVertexData);
    createTileMeshBuffer(frame1Mesh, frame1VertexData);
    createTileMeshBuffer(frame2Mesh, frame2VertexData);
    createTileMeshBuffer(scrollFrame1Mesh, scrollFrame1VertexData);
    createTileMeshBuffer(scrollFrame2Mesh, scrollFrame2VertexData);

    mapMesh.staticMesh = staticMesh;
    mapMesh.animatedMeshes[0] = frame1Mesh;
    mapMesh.animatedMeshes[1] = frame2Mesh;
    mapMesh.scrollMeshes[0] = scrollFrame1Mesh;
    mapMesh.scrollMeshes[1] = scrollFrame2Mesh;
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename) {
    TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

    std::vector<TileLayerData> tileLayerDatas;

    MapData mapData;

    for (const auto& tileLayer : tmxMap.GetTileLayers()) {
        TileLayerData tileLayerData;

        for (int tileY = 0; tileY < tileLayer.GetHeight(); ++tileY) {
            for (int tileX = 0; tileX < tileLayer.GetWidth(); ++tileX) {
                const TmxMapLib::Tile* tile = tileLayer.GetTile(tileX, tileY);

                if (tile->GetGid() == 0) {
                    continue;
                }

                TileData tileData = {};
                tileData.x = tileX;
                tileData.y = tileY;
                tileData.tilesetId = tile->GetGid() - 1;

                const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

                const TmxMapLib::TilesetTile* tilesetTile =
                    tileset->GetTile(tileData.tilesetId);

                if (tilesetTile) {
                    const TmxMapLib::Animation& animation =
                        tilesetTile->GetAnimation();

                    //  Animated tile
                    if(animation.GetFrameCount() > 0) {
                        tileData.animated = true;

                        int nextFrameId = animation.GetFrame(1).GetTileId();

                        const TmxMapLib::TilesetTile* nextFrameTilesetTile =
                            tileset->GetTile(nextFrameId);

                        tileData.nextFrame = nextFrameTilesetTile->GetId();
                    }
                }

                tileLayerData.tiles.push_back(tileData);
            }
        }

        mapData.tileLayers.push_back(tileLayerData);
    }

    MapMesh mapMesh;
    buildMapMesh(mapData, mapMesh);

    std::shared_ptr<Map> map =
        std::make_shared<Map>(tmxMap.GetWidth(), tmxMap.GetHeight());

    map->mapMesh = mapMesh;

    return map;
}
}