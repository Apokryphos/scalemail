#include "ambient_light.hpp"
#include "direction_util.hpp"
#include "gl_headers.hpp"
#include "layer.hpp"
#include "light.hpp"
#include "map.hpp"
#include "map_mesh.hpp"
#include "mesh.hpp"
#include "particle_system.hpp"
#include "sprite.hpp"
#include "string_util.hpp"
#include "tileset.hpp"
#include "world.hpp"
#include "TmxMapLib/Map.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>

namespace ScaleMail
{
struct TileData {
	bool alpha;
	bool animated;
	bool flipHorz;
	bool flipVert;
	bool flipDiag;
	bool wallFace;
	bool wallTop;
	int x;
	int y;
	int tilesetId;
	int nextFrame;
	float rotation;
};

struct TileLayerData {
	bool scroll;
	int layerZ;
	std::vector<TileData> tiles;
};

struct MapData {
	std::vector<TileLayerData> tileLayers;
};

static const glm::vec2 gQuadVertices[] =
{
	glm::vec2(-0.5f, -0.5f),
	glm::vec2(-0.5f,  0.5f),
	glm::vec2( 0.5f,  0.5f),
	glm::vec2( 0.5f, -0.5f),
};

//  ============================================================================
static int getScrollTilesetIdOffset(int tilesetId) {
	if (tilesetId >= 140 && tilesetId <= 143) {
		return 140;
	}

	if (tilesetId >= 152 && tilesetId <= 155) {
		return 148;
	}

	return 0;
}

//  ============================================================================
static bool getTilesetAlpha(const TmxMapLib::Map& tmxMap, const int gid) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - 1);

	if (tilesetTile) {
		return tilesetTile->GetPropertySet().GetBoolValue("AlphaTest", false);
	}

	return false;
}

//  ============================================================================
static bool getTilesetAnimation(const TmxMapLib::Map& tmxMap, const int gid,
								int& nextFrame) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - 1);

	if (tilesetTile) {
		const TmxMapLib::Animation& animation = tilesetTile->GetAnimation();

		if(animation.GetFrameCount() > 0) {
			int nextFrameId = animation.GetFrame(1).GetTileId();

			const TmxMapLib::TilesetTile* nextFrameTilesetTile =
				tileset->GetTile(nextFrameId);

			nextFrame = nextFrameTilesetTile->GetId();

			return true;
		}
	}

	return false;
}

//  ============================================================================
static void loadTileData(TileData& tileData, const TmxMapLib::Map& tmxMap,
						 const int gid) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - 1);

	if (tilesetTile) {
		tileData.wallFace =
			tilesetTile->GetPropertySet().GetBoolValue("WallFace", false);

		tileData.wallTop =
			tilesetTile->GetPropertySet().GetBoolValue("WallTop", false);
	}
}

//  ============================================================================
static float getDecal(const TmxMapLib::Map& tmxMap, const int gid) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - 1);

	if (tilesetTile) {
		return tilesetTile->GetPropertySet().GetBoolValue("Decal", false);
	}

	return false;
}

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

	if (!tile.flipVert) {
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

	const int elementCount = 5;

	mesh.vertexCount = meshVertexData.size() / elementCount;
	mesh.vertexBufferSize = meshVertexData.size();

	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	size_t vertexDataSize = sizeof(float) * meshVertexData.size();

	glBufferData(GL_ARRAY_BUFFER, vertexDataSize,
				 &meshVertexData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
						  sizeof(float) * elementCount, (void*) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
						  sizeof(float) * elementCount, (void*) (sizeof(float) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//  ============================================================================
static void buildMapMesh(MapData& mapData, MapMesh& mapMesh) {
	std::vector<float> staticVertexData;
	std::vector<float> alphaVertexData;
	std::vector<float> frame1VertexData;
	std::vector<float> frame2VertexData;
	std::vector<float> scrollFrame1VertexData;
	std::vector<float> scrollFrame2VertexData;

	for (auto& tileLayer : mapData.tileLayers) {
		for (auto& tile : tileLayer.tiles) {
			glm::vec3 size =
				glm::vec3(16.0f, 16.0f, 0.0f);

			//	Make a 45 degree wall face
			if (tile.wallFace) {
				size.z = 16.0f;
			}

			glm::vec3 position =
				glm::vec3(tile.x, tile.y, 0.0f) * size;

			//	Make a raised wall top
			if (tile.wallTop) {
				position.z = 17.0f;
			}

			const int MAX_TILE_LAYERS = 10;

			if (!tile.wallTop && !tile.wallFace) {
				position.z = -1.0f + ((float)tileLayer.layerZ / (float)MAX_TILE_LAYERS);
			}

			int tilesetId = tile.tilesetId;

			glm::vec2 uv1, uv2;
			getTilesetUv(tilesetId, 256, 304, 16, 16, uv1, uv2);
			flipTileUv(tile, uv1, uv2);

			if (!tile.animated) {
				addTileVertexData(tile.alpha ? alphaVertexData : staticVertexData,
								  position, size, uv1, uv2,
								  tile.rotation);
			} else {
				if (tileLayer.scroll) {
					int scrollTilesetIdOffset =
						getScrollTilesetIdOffset(tilesetId);

					getTilesetUv(tilesetId - scrollTilesetIdOffset,
								 32, 64, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(scrollFrame1VertexData, position, size,
									  uv1, uv2, tile.rotation);

					getTilesetUv(tile.nextFrame - scrollTilesetIdOffset,
								 32, 64, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(scrollFrame2VertexData, position, size,
									  uv1, uv2, tile.rotation);
				} else {
					addTileVertexData(frame1VertexData, position, size, uv1,
									  uv2, tile.rotation);
					getTilesetUv(tile.nextFrame, 256, 304, 16, 16, uv1, uv2);
					flipTileUv(tile, uv1, uv2);
					addTileVertexData(frame2VertexData, position, size, uv1,
									  uv2, tile.rotation);
				}
			}
		}
	}

	Mesh staticMesh;
	Mesh alphaMesh;
	Mesh frame1Mesh;
	Mesh frame2Mesh;
	Mesh scrollFrame1Mesh;
	Mesh scrollFrame2Mesh;

	createTileMeshBuffer(alphaMesh, alphaVertexData);
	createTileMeshBuffer(staticMesh, staticVertexData);
	createTileMeshBuffer(frame1Mesh, frame1VertexData);
	createTileMeshBuffer(frame2Mesh, frame2VertexData);
	createTileMeshBuffer(scrollFrame1Mesh, scrollFrame1VertexData);
	createTileMeshBuffer(scrollFrame2Mesh, scrollFrame2VertexData);

	mapMesh.alphaMesh = alphaMesh;
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

	const TmxMapLib::PropertySet& propertySet = tilesetTile->GetPropertySet();

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

	const std::string aiName = toLowercase(propertySet.GetValue("Ai", ""));
	const std::string prefabName = toLowercase(propertySet.GetValue("Prefab", ""));

	const float width = object.GetWidth();
	const float height = object.GetHeight();

	Entity entity = world.createActor(object.GetX(), object.GetY(),
									  glm::vec2(width, height), actorIndex,
									  facing, object.GetName(), prefabName,
									  aiName);

	if (object.GetPropertySet().GetBoolValue("Buried", false)) {
		BurySystem& burySystem = world.getBurySystem();
		burySystem.addComponent(entity);
		BuryComponent buryCmpnt = burySystem.getComponent(entity);
		burySystem.setSpawnDirt(buryCmpnt, true);
		burySystem.setDuration(buryCmpnt, world.getRandom().nextFloat(2.0f, 3.0f));
		burySystem.bury(buryCmpnt, true);
	}
}

//  ============================================================================
static void processActorCollisionObject(World& world,
										const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	world.getPhysicsSystem().addStaticActorObstacle(x, y, width, height);
}

//  ============================================================================
static void processAmbientLightObject(const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	glm::vec4 color =
		hexToVec4(object.GetPropertySet().GetValue("Color", "#FFFFFF"));

	addAmbientLight(color, glm::vec4(x, y, width, height));
}

//  ============================================================================
static Rectangle processCameraBoundsObject(const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	return Rectangle(x, y, width, height);
}

//  ============================================================================
static MapCameraPath processCameraPathObject(const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();

	MapCameraPath path = {};

	for (const auto& point : object.GetPoints()) {
		path.points.push_back(glm::vec2(x + point.X, y + point.Y));
	}

	return path;
}

//  ============================================================================
static void processCollisionObject(World& world,
							  	   const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	world.getPhysicsSystem().addStaticObstacle(x, y, width, height);
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
static void processItemObject(World& world,
							   const TmxMapLib::Object& object,
							   const TmxMapLib::Map& tmxMap) {
	auto const tile = object.GetTile();

	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

	if (tileset == nullptr) {
		std::cout << "Invalid item object in TMX map: no matching tileset." << std::endl;
		return;
	}

	int gid = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(gid);

	const TmxMapLib::PropertySet& propertySet = tilesetTile->GetPropertySet();

	if (tilesetTile == nullptr) {
		std::cout << "Invalid item object in TMX map: no matching tileset tile." << std::endl;
		return;
	}

	const int tilesetId = tile->GetGid() - tileset->GetFirstGid();

	const std::string prefabName = toLowercase(propertySet.GetValue("Prefab", ""));

	const float width = object.GetWidth();
	const float height = object.GetHeight();

	const glm::vec2 position(object.GetX(), object.GetY());

	const glm::vec2 size(width, height);

	const std::string name = object.GetName();

	Entity entity = world.createLoot(position, size, tilesetId, name, prefabName);

	if (object.GetPropertySet().GetBoolValue("Buried", false)) {
		BurySystem& burySystem = world.getBurySystem();
		burySystem.addComponent(entity);
		BuryComponent buryCmpnt = burySystem.getComponent(entity);
		burySystem.setSpawnDirt(buryCmpnt, true);
		burySystem.setDuration(buryCmpnt, world.getRandom().nextFloat(2.0f, 3.0f));
		burySystem.bury(buryCmpnt, true);
	}
}

//  ============================================================================
static void processLightObject(World& world,
							   const TmxMapLib::Object& object) {
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

	bool decal = getDecal(tmxMap, tile->GetGid());

	int nextFrame = tilesetId;
	getTilesetAnimation(tmxMap, tile->GetGid(), nextFrame);

	world.createProp(glm::vec2(x, y), tilesetId, nextFrame, decal);
}

//  ============================================================================
static bool processPlayerStartObject(const TmxMapLib::Object& object,
									 const TmxMapLib::Map& tmxMap,
									 PlayerStart& playerStart) {
	auto const tile = object.GetTile();

	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

	if (tileset == nullptr) {
		std::cout << "Invalid player start object in TMX map: no matching tileset." << std::endl;
		return false;
	}

	int gid = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(gid);

	if (tilesetTile == nullptr) {
		std::cout << "Invalid player start object in TMX map: no matching tileset tile." << std::endl;
		return false;
	}

	const TmxMapLib::Property* actorIndexProperty =
		tilesetTile->GetPropertySet().GetProperty("ActorIndex");

	if (actorIndexProperty == nullptr) {
		std::cout << "Invalid player start object in TMX map: no tileset tile with ActorIndex property." << std::endl;
		return false;
	}

	int actorIndex = actorIndexProperty->GetIntValue(0);

	const TmxMapLib::Property* facingProperty =
		tilesetTile->GetPropertySet().GetProperty("Facing");

	Direction facing = Direction::SOUTH;

	if (facingProperty != nullptr) {
		const std::string value = facingProperty->GetValue();
		facing = stringToDirection(value);
	}

	playerStart.position = glm::vec2(object.GetX(), object.GetY());
	playerStart.actorIndex = actorIndex;
	playerStart.facing = facing;
	return true;
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

	int nextFrame = tilesetId;
	getTilesetAnimation(tmxMap, tile->GetGid(), nextFrame);

	const float x = object.GetX();
	const float y = object.GetY();

	bool decal = getDecal(tmxMap, tile->GetGid());

	Entity entity = world.createProp(glm::vec2(x, y), tilesetId, nextFrame,
									 decal);

	LightSystem& lightSystem = world.getLightSystem();
	lightSystem.addComponent(entity);

	LightComponent lightCmpnt = lightSystem.getComponent(entity);

	lightSystem.setOffset(lightCmpnt, glm::vec2(0, 1));
	lightSystem.setColor(lightCmpnt, torchLightColor);
	lightSystem.setGlowSize(lightCmpnt, torchLightGlowSize);
	lightSystem.setSize(lightCmpnt, torchLightSize);
	lightSystem.setPulse(lightCmpnt, torchLightPulse);
	lightSystem.setPulseSize(lightCmpnt, torchLightPulseSize);

	ParticleSystem& particleSystem = world.getParticleSystem();
	particleSystem.addComponent(entity);

	ParticleComponent particleCmpnt = particleSystem.getComponent(entity);

	ParticleComponentData emitter = {};
	emitter.life = 1.0f;
	emitter.decay = 1.0f;
	emitter.duration = 1.0f;
	emitter.emitCount = 3;
	emitter.interval = 0.16f;
	emitter.minSize = 0.5f;
	emitter.maxSize = 2.0f;
	emitter.minSpeed = 4.0f;
	emitter.maxSpeed = 8.0f;
	emitter.spread = 0.349066f;
	emitter.direction = glm::vec3(0.0f, -1.0f, 2.0f);
	emitter.color = torchLightColor;
	emitter.width = 4.0f;

	particleSystem.setData(particleCmpnt, emitter);
}

//  ============================================================================
static void processTriggerObject(World& world,
								 const TmxMapLib::Object& object) {
	if (!object.GetPropertySet().HasProperty("TargetName")) {
		std::cout << "Trigger object is missing TargetName property." << std::endl;
		return;
	}

	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	const std::string targetName =
		object.GetPropertySet().GetValue("TargetName", "");

	world.createTrigger(x, y, width, height, targetName);
}

//  ============================================================================
static void processObject(World& world,
						  const TmxMapLib::Object& object,
						  const TmxMapLib::Map& tmxMap,
						  std::vector<Rectangle>& cameraBounds,
						  std::vector<MapCameraPath>& cameraPaths,
						  std::vector<PlayerStart>& playerStarts) {
	const std::string type = toLowercase(object.GetType());

	if (object.GetObjectType() == TmxMapLib::ObjectType::Ellipse) {
		if (type == "light") {
			processLightObject(world, object);
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
		} else if (type == "item") {
			processItemObject(world, object, tmxMap);
		} else if (type == "playerstart") {
			PlayerStart playerStart;
			if (processPlayerStartObject(object, tmxMap, playerStart)) {
				playerStarts.push_back(playerStart);
			}
		} else if (type == "torch") {
			processTorchObject(world, object, tmxMap);
		} else {
			processMiscObject(world, object, tmxMap);
		}
	} else if (object.GetObjectType() == TmxMapLib::ObjectType::Basic) {
		if (type == "collision") {
			processCollisionObject(world, object);
		} else if (type == "actorcollision") {
			processActorCollisionObject(world, object);
		} else if (type == "ambientlight") {
			processAmbientLightObject(object);
		} else if (type == "camerabounds") {
			cameraBounds.push_back(processCameraBoundsObject(object));
		} else if (type == "trigger") {
			processTriggerObject(world, object);
		}
	} else if (object.GetObjectType() == TmxMapLib::ObjectType::Polyline) {
		if (type == "camerapath") {
			cameraPaths.push_back(processCameraPathObject(object));
		}
	}
}

//  ============================================================================
static void processObjects(const TmxMapLib::Map tmxMap, World& world,
						   std::vector<MapCamera>& mapCameras,
						   std::vector<PlayerStart>& playerStarts) {
	for (const auto& objectGroup : tmxMap.GetObjectGroups()) {
		std::vector<Rectangle> cameraBounds;
		std::vector<MapCameraPath> cameraPaths;

		for (const auto& object : objectGroup.GetObjects()) {
			processObject(world, object, tmxMap, cameraBounds, cameraPaths,
						  playerStarts);
		}

		if (cameraBounds.size() > 0 || cameraPaths.size() > 0) {
			MapCamera mapCamera = {};
			mapCamera.name = objectGroup.GetName();
			mapCamera.bounds = cameraBounds;
			mapCamera.paths = cameraPaths;

			mapCameras.push_back(mapCamera);
		}
	}
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename, World& world) {
	world.getPhysicsSystem().clearStaticObstacles();

	TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

	initializeLayers(tmxMap.GetHeight() * tmxMap.GetTileHeight());

	std::vector<MapCamera> mapCameras;
	std::vector<PlayerStart> playerStarts;
	processObjects(tmxMap, world, mapCameras, playerStarts);

	std::vector<TileLayerData> tileLayerDatas;

	MapData mapData;

	int layerIndex = 0;
	for (const auto& tileLayer : tmxMap.GetTileLayers()) {
		TileLayerData tileLayerData;

		tileLayerData.layerZ =
			tileLayer.GetPropertySet().GetIntValue("LayerZ", layerIndex);

		tileLayerData.scroll =
			tileLayer.GetPropertySet().GetBoolValue("LayerScroll", false);

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

				tileData.alpha = getTilesetAlpha(tmxMap, tile->GetGid());

				loadTileData(tileData, tmxMap, tile->GetGid());

				int nextFrame;
				if (getTilesetAnimation(tmxMap, tile->GetGid(), nextFrame)) {
					tileData.animated = true;
					tileData.nextFrame = nextFrame;
				}

				tileLayerData.tiles.push_back(tileData);
			}
		}

		mapData.tileLayers.push_back(tileLayerData);

		++layerIndex;
	}

	MapMesh mapMesh;
	buildMapMesh(mapData, mapMesh);

	std::shared_ptr<Map> map =
		std::make_shared<Map>(
			tmxMap.GetWidth(), tmxMap.GetHeight(),
			tmxMap.GetTileWidth(), tmxMap.GetTileHeight());

	map->mapMesh = mapMesh;

	map->setPlayerStarts(playerStarts);

	for (const MapCamera& mapCamera : mapCameras) {
		map->addCamera(mapCamera);
	}

	return map;
}
}