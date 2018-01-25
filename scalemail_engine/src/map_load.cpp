#include "direction_util.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "map.hpp"
#include "mesh.hpp"
#include "sprite.hpp"
#include "string_util.hpp"
#include "tileset.hpp"
#include "world.hpp"
#include "TmxMapLib/Map.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
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
static glm::vec4 hexToVec4(std::string input) {

    if (input[0] == '#') {
        input.erase(0, 1);
    }

    unsigned long value = stoul(input, nullptr, 16);

    glm::vec4 color;

    color.a = (value >> 24) & 0xff;
    color.r = (value >> 16) & 0xff;
    color.g = (value >> 8) & 0xff;
    color.b = (value >> 0) & 0xff;

    return color / 255.0f;
}

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
static void processActorObject(World& world,
                               const TmxMapLib::Object& object,
                               const TmxMapLib::Map& tmxMap) {
    auto const tile = object.GetTile();

    const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

    if (tileset == nullptr) {
        std::cout << "Invalid actor object in TMX map: no matching tileset." << std::endl;
        return;
    }

    int gid = tile->GetGid() - tileset->GetFirstGid();

    const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(gid);

    if (tilesetTile == nullptr) {
        std::cout << "Invalid actor object in TMX map: no matching tileset tile." << std::endl;
        return;
    }

    const TmxMapLib::Property* actorIndexProperty =
        tilesetTile->GetPropertySet().GetProperty("ActorIndex");

    if (actorIndexProperty == nullptr) {
        std::cout << "Invalid actor object in TMX map: no tileset tile with ActorIndex property." << std::endl;
        return;
    }

    int actorIndex = actorIndexProperty->GetIntValue(0);

    const TmxMapLib::Property* facingProperty =
        tilesetTile->GetPropertySet().GetProperty("Facing");

    Direction facing = Direction::SOUTH;

    if (facingProperty != nullptr) {
        const std::string value = facingProperty->GetValue();
        facing = stringToDirection(value);
    }

    world.createActor(object.GetX(), object.GetY(), actorIndex, facing);
}

//  ============================================================================
static void processLightObject(World& world,
                               const TmxMapLib::Object& object,
                               const TmxMapLib::Map& tmxMap) {
    //  Scale light size by constant...lights are too small
    float lightSize = object.GetWidth() * 2.5f;

    const auto& propertySet = object.GetPropertySet();

    std::string hex = propertySet.GetValue("LightColor", "#FFFFFFFF");
    glm::vec4 lightColor = hexToVec4(hex);

    float lightPulse = propertySet.GetFloatValue("LightPulse", 0);

    float lightPulseSize = propertySet.GetFloatValue("LightPulseSize", 0);

    const float x = object.GetX() + object.GetWidth() * 0.5f;
    const float y = object.GetY() + object.GetHeight() * 0.5f;

    addLight(
        glm::vec2(x, y),
        lightColor,
        lightSize,
        lightPulse,
        lightPulseSize
    );
}

//  ============================================================================
static void processMiscObject(World& world,
                              const TmxMapLib::Object& object,
                              const TmxMapLib::Map& tmxMap) {
    const float x = object.GetX();
    const float y = object.GetY();

    auto const tile = object.GetTile();

    const int tilesetId = tile->GetGid() - 1;

    addWorldSprite(glm::vec2(x, y), tilesetId);
}

//  ============================================================================
static void processTorchObject(World& world,
                               const TmxMapLib::Object& object,
                               const TmxMapLib::Map& tmxMap) {
    const glm::vec4 torchLightColor(1.0f, 0.6f, 0.0f, 0.75f);
    const float torchLightSize = 64;
    const float torchLightPulse = 8;
    const float torchLightPulseSize = 8;

    auto const tile = object.GetTile();

    const int tilesetId = tile->GetGid() - 1;

    const float x = object.GetX();
    const float y = object.GetY();

    addLight(
        glm::vec2(x + 8.0f, y - 7.0f),
        torchLightColor,
        torchLightSize,
        torchLightPulse,
        torchLightPulseSize
    );

    addWorldSprite(glm::vec2(x, y), tilesetId);
}

//  ============================================================================
static void processObject(World& world,
                          const TmxMapLib::Object& object,
                          const TmxMapLib::Map& tmxMap) {
    const std::string type = toLowercase(object.GetType());

    if (object.GetObjectType() == TmxMapLib::ObjectType::Ellipse) {
        if (type == "light") {
            processLightObject(world, object, tmxMap);
        } else {
            std::cout << "Unrecognized ellipse map object." << std::endl;
        }
    }

    if (object.GetObjectType() == TmxMapLib::ObjectType::Tile) {
        if (object.GetTile() == nullptr) {
            std::cout << "Map object is not a tile object." << std::endl;
        }

        if (type == "actor") {
            processActorObject(world, object, tmxMap);
        } else if (type == "torch") {
            processTorchObject(world, object, tmxMap);
        } else {
            processMiscObject(world, object, tmxMap);
        }
    }
}

//  ============================================================================
static void processObjects(const TmxMapLib::Map tmxMap, World& world) {
    for (const auto& objectGroup : tmxMap.GetObjectGroups()) {
        for (const auto& object : objectGroup.GetObjects()) {
            processObject(world, object, tmxMap);
        }
    }
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename, World& world) {
    TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

    processObjects(tmxMap, world);

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