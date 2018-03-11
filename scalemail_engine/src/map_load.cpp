#include "ai_system.hpp"
#include "ambient_light.hpp"
#include "direction_util.hpp"
#include "entity_util.hpp"
#include "entity_types.hpp"
#include "gl_headers.hpp"
#include "layer.hpp"
#include "light.hpp"
#include "light_system.hpp"
#include "map.hpp"
#include "map_mesh.hpp"
#include "mesh.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "random.hpp"
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
struct TileData
{
	bool alpha;
	bool animated;
	bool flipHorz;
	bool flipVert;
	bool flipDiag;
	bool wallFace;
	bool wallTop;
	int nextFrame;
	int tilesetId;
	int x;
	int y;
};

struct TileLayerData
{
	bool scroll;
	int layerZ;
	std::vector<TileData> tiles;
};

struct MapData
{
	std::vector<AmbientLight> ambientLights;
	std::vector<MapCamera> mapCameras;
	std::vector<PlayerStart> playerStarts;
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
static void buryEntity(const TmxMapLib::Object& object, const Entity& entity,
					   World& world) {
	if (object.GetPropertySet().GetBoolValue("Buried", false)) {
		buryEntity(entity, world.getBurySystem(), true,
			world.getRandom().nextFloat(4.0f, 5.0f), true);
	}
}

//  ============================================================================
static glm::vec2 getTileObjectPosition(const TmxMapLib::Object& object) {
	return glm::vec2(
		object.GetX() + object.GetWidth() * 0.5f,
		object.GetY() - object.GetHeight() * 0.5f);
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

			//	Use alias as tileset ID if present
			nextFrame =
				nextFrameTilesetTile->GetPropertySet().GetIntValue(
					"AliasTilesetId", nextFrameTilesetTile->GetId());

			return true;
		}
	}

	return false;
}

//  ============================================================================
static void loadTileDataProperties(TileData& tileData,
								   const TmxMapLib::TilesetTile& tilesetTile) {
	const TmxMapLib::PropertySet& propertySet = tilesetTile.GetPropertySet();

	tileData.alpha =
		propertySet.GetBoolValue("AlphaTest", false);

	tileData.wallFace =
		propertySet.GetBoolValue("WallFace", false);

	tileData.wallTop =
		propertySet.GetBoolValue("WallTop", false);

	//	Use alias as tileset ID if present
	tileData.tilesetId =
		propertySet.GetIntValue("AliasTilesetId", tileData.tilesetId);
}

//  ============================================================================
static TileData loadTileData(const TmxMapLib::Map& tmxMap,
							 const TmxMapLib::Tile& tile) {
	TileData tileData = {};
	tileData.x = tile.GetX();
	tileData.y = tile.GetY();
	tileData.flipHorz = tile.GetFlipHorizontally();
	tileData.flipVert = tile.GetFlipVertically();
	tileData.flipDiag = tile.GetFlipDiagonally();

	int gid = tile.GetGid();

	//	Set initial tileset ID
	tileData.tilesetId = gid - 1;

	//	Set initial animation
	tileData.nextFrame = tileData.tilesetId;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrame);

	//	Load properties from tileset tile
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(gid - 1);

	//	Return early if there's no matching tileset tile
	if (tilesetTile == nullptr) {
		return tileData;
	}

	const TmxMapLib::PropertySet& propertySet = tilesetTile->GetPropertySet();

	//	Is this is an odd/even tile (like a checkerboard)?
	const bool odd = (tileData.x + tileData.y) % 2;

	int tilesetId = gid - 1;

	//	Set tileset ID to alternate odd/even ID if it is specified
	if (odd) {
		tilesetId = propertySet.GetIntValue("OddTilesetId", tilesetId);
	} else {
		tilesetId = propertySet.GetIntValue("EvenTilesetId", tilesetId);
	}

	gid = tileset->GetFirstGid() + tilesetId;

	//	Update to actual tileset ID
	tileData.tilesetId = tilesetId;

	//	Update animation to actual tileset ID
	tileData.nextFrame = tilesetId;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrame);

	//	Get the actual tileset tile to use for properties
	const TmxMapLib::TilesetTile* finalTilesetTile =
		tileset->GetTile(tilesetId);

	if (finalTilesetTile != nullptr) {
		loadTileDataProperties(tileData, *finalTilesetTile);
	}

	return tileData;
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
static void createTileMeshBuffer(Mesh& mesh, std::vector<float>& meshVertexData,
								 const RenderOptions& renderOptions) {
	initMesh(mesh, VertexDefinition::POSITION3_TEXTURE2, renderOptions);
	setMeshVertexData(mesh, meshVertexData);
}

//  ============================================================================
static void buildMapMesh(MapData& mapData, MapMesh& mapMesh,
						 const RenderOptions& renderOptions) {
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

			if (!tile.animated) {
				addTileVertexData(tile.alpha ? alphaVertexData : staticVertexData,
								  position, size, uv1, uv2, tile.flipDiag,
								  tile.flipHorz, tile.flipVert);
			} else {
				if (tileLayer.scroll) {
					const int scrollTextureWidth = 32;
					const int scrollTextureHeight = 128;
					const int scrollTileWidth = 16;
					const int scrollTileHeight = 16;

					getTilesetUv(tilesetId, scrollTextureWidth,
								 scrollTextureHeight, scrollTileWidth,
								 scrollTileHeight, uv1, uv2);

					addTileVertexData(scrollFrame1VertexData, position, size,
									  uv1, uv2, tile.flipDiag, tile.flipHorz,
									  tile.flipVert);

					getTilesetUv(tile.nextFrame, scrollTextureWidth,
								 scrollTextureHeight, scrollTileWidth,
								 scrollTileHeight, uv1, uv2);

					addTileVertexData(scrollFrame2VertexData, position, size,
									  uv1, uv2, tile.flipDiag, tile.flipHorz,
									  tile.flipVert);
				} else {
					addTileVertexData(frame1VertexData, position, size, uv1,
									  uv2, tile.flipDiag, tile.flipHorz,
									  tile.flipVert);

					getTilesetUv(tile.nextFrame, 256, 304, 16, 16, uv1, uv2);

					addTileVertexData(frame2VertexData, position, size, uv1,
									  uv2, tile.flipDiag, tile.flipHorz,
									  tile.flipVert);
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

	createTileMeshBuffer(alphaMesh, alphaVertexData, renderOptions);
	createTileMeshBuffer(staticMesh, staticVertexData, renderOptions);
	createTileMeshBuffer(frame1Mesh, frame1VertexData, renderOptions);
	createTileMeshBuffer(frame2Mesh, frame2VertexData, renderOptions);
	createTileMeshBuffer(scrollFrame1Mesh, scrollFrame1VertexData, renderOptions);
	createTileMeshBuffer(scrollFrame2Mesh, scrollFrame2VertexData, renderOptions);

	mapMesh.alphaMesh = alphaMesh;
	mapMesh.staticMesh = staticMesh;
	mapMesh.animatedMeshes[0] = frame1Mesh;
	mapMesh.animatedMeshes[1] = frame2Mesh;
	mapMesh.scrollMeshes[0] = scrollFrame1Mesh;
	mapMesh.scrollMeshes[1] = scrollFrame2Mesh;
}

//  ============================================================================
static void processVillainObject(World& world,
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

	const glm::vec2 position = getTileObjectPosition(object);
	const glm::vec2 size(object.GetWidth(), object.GetHeight());

	Entity entity = createVillainActor(world, position, size, actorIndex,
									   facing, object.GetName(), prefabName,
									   aiName);

	buryEntity(object, entity, world);
}

//  ============================================================================
static void processActorCollisionObject(World& world,
										const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	world.getPhysicsSystem().addStaticActorObstacle(x, y, width, height);

	world.getAiSystem().addStaticActorObstacle(x, y, width, height);
}

//  ============================================================================
static void processAmbientLightObject(const TmxMapLib::Object& object,
									  MapData& mapData) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	glm::vec4 color =
		hexToVec4(object.GetPropertySet().GetValue("Color", "#FFFFFF"));

	mapData.ambientLights.push_back({color, glm::vec4(x, y, width, height)});
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

	world.getAiSystem().addStaticObstacle(x, y, width, height);
}

//  ============================================================================
static void processDoorObject(World& world,
							  const TmxMapLib::Object& object,
							  const TmxMapLib::Map& tmxMap) {
	const glm::vec2 position = getTileObjectPosition(object);

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
			createDoor(world, position, openTilesetId, closedTilesetId,
					   openTilesetId + 1, closedTilesetId + 1, open,
					   object.GetName());
		} else {
			createDoor(world, position, openTilesetId, closedTilesetId,
					   open, object.GetName());
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

	const glm::vec2 size(object.GetWidth(), object.GetHeight());

	const glm::vec2 position = getTileObjectPosition(object);

	const std::string name = object.GetName();

	Entity entity =
		createLoot(world, position, size, tilesetId, name, prefabName);

	buryEntity(object, entity, world);
}

//  ============================================================================
static void processLightObject(World& world,
							   const TmxMapLib::Object& object) {
	//  Scale light size by constant...lights are too small
	glm::vec2 lightSize =
		glm::vec2(object.GetWidth(), object.GetHeight()) * 2.5f;

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
	const glm::vec2 position = getTileObjectPosition(object);

	auto const tile = object.GetTile();
	const int tilesetId = tile->GetGid() - 1;

	bool decal = getDecal(tmxMap, tile->GetGid());

	int nextFrame = tilesetId;
	getTilesetAnimation(tmxMap, tile->GetGid(), nextFrame);

	createProp(world, position, tilesetId, nextFrame, decal);
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
	auto const tile = object.GetTile();

	const int tilesetId = tile->GetGid() - 1;

	int nextFrame = tilesetId;
	getTilesetAnimation(tmxMap, tile->GetGid(), nextFrame);

	const glm::vec2 position = getTileObjectPosition(object);

	createTorch(world, position, tilesetId, nextFrame);
}

//  ============================================================================
static void processTriggerObject(World& world,
								 const TmxMapLib::Object& object) {
	if (!object.GetPropertySet().HasProperty("TargetName")) {
		std::cout << "Trigger object is missing TargetName property." << std::endl;
		return;
	}

	const glm::vec2 position(object.GetX(), object.GetY());
	const glm::vec2 size(object.GetWidth(), object.GetHeight());

	const std::string targetName =
		object.GetPropertySet().GetValue("TargetName", "");

	createTrigger(world, position, size, targetName);
}

//  ============================================================================
static void processObject(World& world,
						  const TmxMapLib::Object& object,
						  const TmxMapLib::Map& tmxMap,
						  MapData& mapData,
						  std::vector<Rectangle>& cameraBounds,
						  std::vector<MapCameraPath>& cameraPaths) {
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

		if (type == "villain") {
			processVillainObject(world, object, tmxMap);
		} else if (type == "door") {
			processDoorObject(world, object, tmxMap);
		} else if (type == "item") {
			processItemObject(world, object, tmxMap);
		} else if (type == "playerstart") {
			PlayerStart playerStart;
			if (processPlayerStartObject(object, tmxMap, playerStart)) {
				mapData.playerStarts.push_back(playerStart);
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
			processAmbientLightObject(object, mapData);
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
						   MapData& mapData) {
	for (const auto& objectGroup : tmxMap.GetObjectGroups()) {
		std::vector<Rectangle> cameraBounds;
		std::vector<MapCameraPath> cameraPaths;

		for (const auto& object : objectGroup.GetObjects()) {
			processObject(world, object, tmxMap, mapData, cameraBounds, cameraPaths);
		}

		if (cameraBounds.size() > 0 || cameraPaths.size() > 0) {
			MapCamera mapCamera = {};
			mapCamera.name = objectGroup.GetName();
			mapCamera.bounds = cameraBounds;
			mapCamera.paths = cameraPaths;

			mapData.mapCameras.push_back(mapCamera);
		}
	}
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename, World& world,
							 const RenderOptions& renderOptions) {
	world.getPhysicsSystem().clearStaticObstacles();

	TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

	initializeLayers(tmxMap.GetHeight() * tmxMap.GetTileHeight());

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

				TileData tileData = loadTileData(tmxMap, *tile);

				tileLayerData.tiles.push_back(tileData);
			}
		}

		mapData.tileLayers.push_back(tileLayerData);

		++layerIndex;
	}

	processObjects(tmxMap, world, mapData);

	MapMesh mapMesh;
	buildMapMesh(mapData, mapMesh, renderOptions);

	std::shared_ptr<Map> map =
		std::make_shared<Map>(
			tmxMap.GetWidth(), tmxMap.GetHeight(),
			tmxMap.GetTileWidth(), tmxMap.GetTileHeight());

	map->mapMesh = mapMesh;

	map->setAmbientLights(mapData.ambientLights);
	map->setPlayerStarts(mapData.playerStarts);

	for (const MapCamera& mapCamera : mapData.mapCameras) {
		map->addCamera(mapCamera);
	}

	return map;
}
}