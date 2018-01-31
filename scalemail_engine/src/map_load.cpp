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
	bool flipHorz;
	bool flipVert;
	bool flipDiag;
	int x;
	int y;
	int tilesetId;
	int nextFrame;
	float rotation;
};

struct TileLayerData {
	std::vector<TileData> tiles;
};

struct MapData {
	std::vector<TileLayerData> tileLayers;
};

static const glm::vec2 gQuadVertices[] =
{
	glm::vec2(-0.5f, -0.5f),
	glm::vec2(-0.5f, 0.5f),
	glm::vec2(0.5f,	0.5f),
	glm::vec2(0.5f,	-0.5f),
};

//  ============================================================================
static void flipTileUv(TileData& tile, glm::vec2& uv1, glm::vec2& uv2) {
	const float piOver2 = 3.14159265359f;

	tile.rotation = 0;

	if (tile.flipDiag) {
		if (tile.flipHorz && tile.flipVert)
		{
			tile.rotation = piOver2;
			tile.flipVert = false;
		}
		else if (tile.flipHorz)
		{
			tile.rotation = -piOver2;
			tile.flipVert = !tile.flipVert;
		}
		else if (tile.flipVert)
		{
			tile.rotation = piOver2;
			tile.flipHorz = false;
		}
		else
		{
			tile.rotation = -piOver2;
			tile.flipHorz = !tile.flipHorz;
		}
	}

	if (tile.flipHorz) {
		std::swap(uv1.x, uv2.x);
	}

	if (tile.flipVert) {
		std::swap(uv1.y, uv2.y);
	}

	tile.rotation = 0;
}

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
							  const glm::vec2 uv1, const glm::vec2 uv2,
							  float rotate) {
	float u1 = uv1.x;
	float v1 = uv1.y;
	float u2 = uv2.x;
	float v2 = uv2.y;

	float sinAngle = sin(rotate);
	float cosAngle = cos(rotate);
	glm::mat2 rotation = glm::mat2(
		cosAngle, -sinAngle,
		sinAngle,  cosAngle);

	glm::vec2 offset = glm::vec2(8.f, 8.0f);

	glm::vec2 quadA = position + offset + rotation * gQuadVertices[0] * size;
	glm::vec2 quadB = position + offset + rotation * gQuadVertices[1] * size;
	glm::vec2 quadC = position + offset + rotation * gQuadVertices[2] * size;
	glm::vec2 quadD = position + offset + rotation * gQuadVertices[3] * size;

	meshVertexData.push_back(quadA.x);
	meshVertexData.push_back(quadA.y);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v1);

	meshVertexData.push_back(quadB.x);
	meshVertexData.push_back(quadB.y);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadC.x);
	meshVertexData.push_back(quadC.y);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadA.x);
	meshVertexData.push_back(quadA.y);
	meshVertexData.push_back(u1);
	meshVertexData.push_back(v1);

	meshVertexData.push_back(quadC.x);
	meshVertexData.push_back(quadC.y);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v2);

	meshVertexData.push_back(quadD.x);
	meshVertexData.push_back(quadD.y);
	meshVertexData.push_back(u2);
	meshVertexData.push_back(v1);
}

//  ============================================================================
static void buildMapMesh(MapData& mapData, MapMesh& mapMesh) {
	std::vector<float> staticVertexData;
	std::vector<float> frame1VertexData;
	std::vector<float> frame2VertexData;
	std::vector<float> scrollFrame1VertexData;
	std::vector<float> scrollFrame2VertexData;

	for (auto& tileLayer : mapData.tileLayers) {
		for (auto& tile : tileLayer.tiles) {
			glm::vec2 size = glm::vec2(16);

			glm::vec2 position = glm::vec2(tile.x, tile.y) * size;

			int tilesetId = tile.tilesetId;

			glm::vec2 uv1, uv2;
			getTilesetUv(tilesetId, 256, 304, 16, 16, uv1, uv2);
			flipTileUv(tile, uv1, uv2);

			if (!tile.animated) {
				addTileVertexData(staticVertexData, position, size, uv1, uv2, tile.rotation);
			} else {
				if (tilesetId >= 140 && tilesetId <= 143) {
					getTilesetUv(tilesetId - 140, 32, 32, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(scrollFrame1VertexData, position, size, uv1, uv2, tile.rotation);
					getTilesetUv(tile.nextFrame - 140, 32, 32, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(scrollFrame2VertexData, position, size, uv1, uv2, tile.rotation);
				} else {
					addTileVertexData(frame1VertexData, position, size, uv1, uv2, tile.rotation);
					getTilesetUv(tile.nextFrame, 256, 304, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(frame2VertexData, position, size, uv1, uv2, tile.rotation);
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

	world.createActor(object.GetX(), object.GetY(), actorIndex, facing,
					  object.GetName());
}

//  ============================================================================
static void processDoorObject(World& world,
							  const TmxMapLib::Object& object,
							  const TmxMapLib::Map& tmxMap) {
	const float x = object.GetX();
	const float y = object.GetY();

	const TmxMapLib::Tile* tile = object.GetTile();

	const int tilesetId = tile->GetGid() - 1;

	bool open = false;

	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(tilesetId);

	if (tilesetTile != nullptr) {
		open = tilesetTile->GetPropertySet().GetBoolValue("DoorOpen", false);

		int openTilesetId = tilesetId;
		int closedTilesetId = tilesetId;

		if (open) {
			closedTilesetId = tilesetTile->GetPropertySet().GetIntValue(
				"DoorClosedId", tilesetId);
		} else {
			openTilesetId = tilesetTile->GetPropertySet().GetIntValue(
				"DoorOpenId", tilesetId);
		}

		if (openTilesetId == closedTilesetId) {
			std::cout <<
				"Door object does not have correct " <<
				(open ? "DoorClosedId" : "DoorOpenId") <<
				" property." << std::endl;
			return;
		}

		if (tilesetTile->GetPropertySet().GetBoolValue("DoorChild", false)) {
			world.createDoor(x, y, openTilesetId, closedTilesetId,
					openTilesetId + 1, closedTilesetId + 1,
					open, object.GetName());
		} else {
			world.createDoor(x, y, openTilesetId, closedTilesetId, open, object.GetName());
		}

	} else {
		std::cout << "Door object missing tileset properties.";
	}
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

	Entity entity = world.createEntity();

	PhysicsSystem& physicsSystem = world.getPhysicsSystem();
	physicsSystem.addComponent(entity);
	PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
	physicsSystem.setPosition(physicsCmpnt, glm::vec2(x, y));

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);
	LightComponent lightCmpnt = lightSystem.getComponent(entity);
	lightSystem.setColor(lightCmpnt, lightColor);
	lightSystem.setGlowSize(lightCmpnt, lightSize * 0.25f);
	lightSystem.setSize(lightCmpnt, lightSize);
	lightSystem.setPulse(lightCmpnt, lightPulse);
	lightSystem.setPulseSize(lightCmpnt, lightPulseSize);
}

//  ============================================================================
static void processMiscObject(World& world,
							  const TmxMapLib::Object& object,
							  const TmxMapLib::Map& tmxMap) {
	const float x = object.GetX();
	const float y = object.GetY();

	auto const tile = object.GetTile();

	const int tilesetId = tile->GetGid() - 1;

	world.createProp(glm::vec2(x, y), tilesetId);
}

//  ============================================================================
static void processTorchObject(World& world,
							   const TmxMapLib::Object& object,
							   const TmxMapLib::Map& tmxMap) {
	const glm::vec4 torchLightColor(1.0f, 0.6f, 0.0f, 0.75f);
	const float torchLightSize = 64;
	const float torchLightGlowSize = torchLightSize * 0.25f;
	const float torchLightPulse = 8;
	const float torchLightPulseSize = 8;

	auto const tile = object.GetTile();

	const int tilesetId = tile->GetGid() - 1;

	const float x = object.GetX();
	const float y = object.GetY();

	Entity entity = world.createProp(glm::vec2(x, y), tilesetId);

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);

	LightComponent lightCmpnt = lightSystem.getComponent(entity);

	lightSystem.setOffset(lightCmpnt, glm::vec2(0, 1));
	lightSystem.setColor(lightCmpnt, torchLightColor);
	lightSystem.setGlowSize(lightCmpnt, torchLightGlowSize);
	lightSystem.setSize(lightCmpnt, torchLightSize);
	lightSystem.setPulse(lightCmpnt, torchLightPulse);
	lightSystem.setPulseSize(lightCmpnt, torchLightPulseSize);
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
		} else if (type == "door") {
			processDoorObject(world, object, tmxMap);
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
				tileData.flipHorz = tile->GetFlipHorizontally();
				tileData.flipVert = tile->GetFlipVertically();
				tileData.flipDiag = tile->GetFlipDiagonally();

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