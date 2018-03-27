#include "ai_system.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "camera_system.hpp"
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
#include "name_system.hpp"
#include "path.hpp"
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
	bool scroll;
	bool wallFace;
	bool wallTop;
	int gid;
	int nextFrameGid;
	int x;
	int y;
};

struct TileLayerData
{
	bool scroll;
	int layerZ;
	std::vector<TileData> tiles;
};

struct TilesetData
{
	int firstGid;
	int tileCount;
	int tileWidth;
	int tileHeight;
	std::string textureName;
};

struct MapData
{
	std::vector<AmbientLight> ambientLights;
	std::vector<PlayerStart> playerStarts;
	std::vector<TileLayerData> tileLayers;
	std::vector<TilesetData> tilesets;
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
//	Get the tileset image source name without the path and extension
static std::string getTilesetImageName(const TmxMapLib::Tileset& tileset) {
	std::string filename = tileset.GetImage().GetSource();

	const size_t lastSlashIndex = filename.find_last_of("\\/");
	if (std::string::npos != lastSlashIndex) {
		filename.erase(0, lastSlashIndex + 1);
	}

	const size_t periodIndex = filename.rfind('.');
	if (std::string::npos != periodIndex) {
		filename.erase(periodIndex);
	}

	return filename;
}

//  ============================================================================
//	Gets a TilesetData by GID
static const TilesetData& getTilesetData(
	const std::vector<TilesetData>& tilesets,
	int gid) {
	const auto tileset = std::find_if(tilesets.begin(), tilesets.end(),
	[gid](auto& ts) {
		return gid >= ts.firstGid &&
			   gid < ts.firstGid + ts.tileCount;
	});

	if (tileset != tilesets.end()) {
		return *tileset;
	}

	throw std::runtime_error("No matching tileset found.");
}

//  ============================================================================
//	Returns a TilesetData for a TmxMapLib Tileset
static const TilesetData getTilesetData(const TmxMapLib::Tileset& tileset) {
	TilesetData tilesetData = {};
	tilesetData.firstGid = tileset.GetFirstGid();
	tilesetData.tileCount = tileset.GetTileCount();
	tilesetData.tileWidth = tileset.GetTileWidth();
	tilesetData.tileHeight = tileset.GetTileHeight();
	tilesetData.textureName = getTilesetImageName(tileset);
	return tilesetData;
}

//  ============================================================================
static glm::vec2 getTileObjectPosition(const TmxMapLib::Object& object) {
	return glm::vec2(
		object.GetX() + object.GetWidth() * 0.5f,
		object.GetY() - object.GetHeight() * 0.5f);
}

//  ============================================================================
static bool getTilesetAnimation(const TmxMapLib::Map& tmxMap, const int gid,
								int& nextFrameGid) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - tileset->GetFirstGid());

	if (tilesetTile) {
		const TmxMapLib::Animation& animation = tilesetTile->GetAnimation();

		if(animation.GetFrameCount() > 0) {
			int nextFrameId = animation.GetFrame(1).GetTileId();

			nextFrameGid = nextFrameId + tileset->GetFirstGid();

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

	if (propertySet.HasProperty("AliasTileset") ||
		propertySet.HasProperty("AliasTilesetId")) {
		throw std::runtime_error("AliasTileset and AliasTilesetId properties are deprecated.");
	}
}

//  ============================================================================
static TileData loadTileData(const TmxMapLib::Map& tmxMap,
							 const TmxMapLib::Tile& tile) {
	int gid = tile.GetGid();

	TileData tileData = {};
	tileData.gid = gid;
	tileData.x = tile.GetX();
	tileData.y = tile.GetY();
	tileData.flipHorz = tile.GetFlipHorizontally();
	tileData.flipVert = tile.GetFlipVertically();
	tileData.flipDiag = tile.GetFlipDiagonally();

	//	Load properties from tileset tile
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	//	Set initial animation
	tileData.nextFrameGid = tileData.gid;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrameGid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(tileData.gid - tileset->GetFirstGid());

	//	Return early if there's no matching tileset tile
	if (tilesetTile == nullptr) {
		return tileData;
	}

	const TmxMapLib::PropertySet& propertySet = tilesetTile->GetPropertySet();

	//	Is this an odd/even tile (like a checkerboard)?
	const bool odd = (tileData.x + tileData.y) % 2;

	//	Set tileset ID to alternate odd/even ID if it is specified
	if (odd && propertySet.HasProperty("OddTilesetId")) {
		gid = propertySet.GetIntValue("OddTilesetId", gid);
		gid += tileset->GetFirstGid();
	} else if (!odd && propertySet.HasProperty("EvenTilesetId")) {
		gid = propertySet.GetIntValue("EvenTilesetId", gid);
		gid += tileset->GetFirstGid();
	}

	//	Update to actual tileset ID
	tileData.gid = gid;

	//	Update animation to actual tileset ID
	tileData.nextFrameGid = gid;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrameGid);

	//	Get the actual tileset tile to use for properties
	const TmxMapLib::TilesetTile* finalTilesetTile =
		tileset->GetTile(gid - tileset->GetFirstGid());

	if (finalTilesetTile != nullptr) {
		loadTileDataProperties(tileData, *finalTilesetTile);
	}

	return tileData;
}

//  ============================================================================
static float getDecal(const TmxMapLib::Map& tmxMap, const int gid) {
	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(gid - tileset->GetFirstGid());

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
static void buildMapMesh(Mesh& mesh, std::vector<float>& meshVertexData,
						 const AssetManager& assetManager) {
	assetManager.initializeMesh(mesh, VertexDefinition::POSITION3_TEXTURE2);
	setMeshVertexData(mesh, meshVertexData);
}

//  ============================================================================
static MapMesh& getMapMesh(TileData& tileData,
						  AssetManager& assetManager,
						  const std::vector<TilesetData>& tilesetDatas,
						  std::vector<MapMesh>& mapMeshes,
						  int frame) {
	auto& tilesetData = getTilesetData(tilesetDatas, tileData.gid);

	auto tileset = assetManager.getTileset(tilesetData.textureName);

	auto itr = std::find_if(mapMeshes.begin(), mapMeshes.end(),
	[frame, &tileData, &tileset](auto& mapMesh) {
		return
			mapMesh.frame == frame &&
			mapMesh.alpha == tileData.alpha &&
			mapMesh.animated == tileData.animated &&
			mapMesh.scroll == tileData.scroll &&
			mapMesh.textureId == tileset.texture.id;
	});

	if (itr != mapMeshes.end()) {
		return *itr;
	}

	MapMesh mapMesh = {};
	mapMesh.alpha = tileData.alpha;
	mapMesh.animated = tileData.animated;
	mapMesh.frame = frame;
	mapMesh.scroll = tileData.scroll;
	mapMesh.textureId = tileset.texture.id;

	mapMeshes.push_back(mapMesh);

	return getMapMesh(tileData,
					  assetManager,
					  tilesetDatas,
					  mapMeshes,
					  frame);
}

//  ============================================================================
static void buildMapMeshes(MapData& mapData, AssetManager& assetManager,
						   std::vector<MapMesh>& mapMeshes) {
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

			const auto& tilesetData = getTilesetData(mapData.tilesets, tile.gid);

			Tileset tileset = assetManager.getTileset(tilesetData.textureName);

			if (tile.animated) {
				glm::vec2 uv1, uv2;
				tileset.getTileUv(tile.nextFrameGid - tilesetData.firstGid, uv1, uv2);

				MapMesh& mapMesh = getMapMesh(
					tile,
					assetManager,
					mapData.tilesets,
					mapMeshes,
					1);

				std::vector<float>& vertexData = mapMesh.vertexData;

				addTileVertexData(vertexData,
								  position, size, uv1, uv2, tile.flipDiag,
								  tile.flipHorz, tile.flipVert);
			}

			glm::vec2 uv1, uv2;
			tileset.getTileUv(tile.gid - tilesetData.firstGid, uv1, uv2);

			MapMesh& mapMesh = getMapMesh(
				tile,
				assetManager,
				mapData.tilesets,
				mapMeshes,
				0);

			std::vector<float>& vertexData = mapMesh.vertexData;

			addTileVertexData(vertexData,
							  position, size, uv1, uv2, tile.flipDiag,
							  tile.flipHorz, tile.flipVert);
		}
	}

	for (auto& mapMesh : mapMeshes) {
		buildMapMesh(mapMesh.mesh, mapMesh.vertexData, assetManager);
	}

	std::sort(
		mapMeshes.begin(),
		mapMeshes.end(),
		[](auto& a, auto& b) {
			return a.textureId < b.textureId;
		}
	);
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

	const int tilesetId = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(tilesetId);

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
									  const Polygon& polygon,
									  MapData& mapData) {
	glm::vec4 color =
		hexToVec4(object.GetPropertySet().GetValue("Color", "#FFFFFF"));

	mapData.ambientLights.push_back({color, polygon});
}

//  ============================================================================
static void processAmbientLightRectangleObject(const TmxMapLib::Object& object,
											   MapData& mapData) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();
	glm::vec4 rect = glm::vec4(x, y, width, height);

	Polygon polygon(rect);

	processAmbientLightObject(object, polygon, mapData);
}

//  ============================================================================
static void processAmbientLightPolygonObject(const TmxMapLib::Object& object,
											 MapData& mapData) {
	const float x = object.GetX();
	const float y = object.GetY();

	std::vector<glm::vec2> points;
	for (const auto& p : object.GetPoints()) {
		points.emplace_back(x + p.X, y + p.Y);
	}

	Polygon polygon(points);

	processAmbientLightObject(object, polygon, mapData);
}

//  ============================================================================
static void processCameraBoundsObject(World& world,
									  const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();
	const float width = object.GetWidth();
	const float height = object.GetHeight();

	const bool visited = object.GetPropertySet().GetBoolValue("Visited", false);

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.addBounds(Rectangle(x, y, width, height), visited);
}

//  ============================================================================
static void processCameraPathObject(World& world,
									const TmxMapLib::Object& object) {
	const float x = object.GetX();
	const float y = object.GetY();

	Path path = {};

	for (const auto& point : object.GetPoints()) {
		path.points.push_back(glm::vec2(x + point.X, y + point.Y));
	}

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.addPath(object.GetName(), path);
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

	const TmxMapLib::Tileset* tileset = tmxMap.GetTilesetByGid(tile->GetGid());

	const int tilesetId = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->GetTile(tilesetId);

	bool open = false;

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

	const int tilesetId = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(tilesetId);

	const TmxMapLib::PropertySet& propertySet = tilesetTile->GetPropertySet();

	if (tilesetTile == nullptr) {
		std::cout << "Invalid item object in TMX map: no matching tileset tile." << std::endl;
		return;
	}

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
	auto const tile = object.GetTile();

	const int gid = tile->GetGid();

	const auto& tileset = tmxMap.GetTilesetByGid(gid);

	const int tilesetId = gid - tileset->GetFirstGid();

	bool decal = getDecal(tmxMap, gid);

	int nextFrameGid = gid;
	getTilesetAnimation(tmxMap, gid, nextFrameGid);

	const int nextFrameTilesetId = nextFrameGid - tileset->GetFirstGid();

	const glm::vec2 position = getTileObjectPosition(object);

	createProp(world, position, tilesetId, nextFrameTilesetId, decal);
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

	int tilesetId = tile->GetGid() - tileset->GetFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->GetTile(tilesetId);

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
	const auto tile = object.GetTile();

	const int gid = tile->GetGid();

	const auto tileset = tmxMap.GetTilesetByGid(gid);

	const int tilesetId = gid - tileset->GetFirstGid();

	int nextFrameGid = gid;
	getTilesetAnimation(tmxMap, gid, nextFrameGid);

	const int nextFrameTilesetId = nextFrameGid - tileset->GetFirstGid();

	const glm::vec2 position = getTileObjectPosition(object);

	createTorch(world, position, tilesetId, nextFrameTilesetId);
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
						  MapData& mapData) {
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
			processAmbientLightRectangleObject(object, mapData);
		} else if (type == "camerabounds") {
			processCameraBoundsObject(world, object);
		} else if (type == "trigger") {
			processTriggerObject(world, object);
		}
	} else if (object.GetObjectType() == TmxMapLib::ObjectType::Polyline) {
		if (type == "camerapath") {
			processCameraPathObject(world, object);
		}
	} else if (object.GetObjectType() == TmxMapLib::ObjectType::Polygon) {
		if (type == "ambientlight") {
			processAmbientLightPolygonObject(object, mapData);
		}
	}
}

//  ============================================================================
static void processObjects(const TmxMapLib::Map tmxMap, World& world,
						   MapData& mapData) {
	for (const auto& objectGroup : tmxMap.GetObjectGroups()) {
		std::vector<Rectangle> cameraBounds;
		std::vector<Path> cameraPaths;

		for (const auto& object : objectGroup.GetObjects()) {
			processObject(world, object, tmxMap, mapData);
		}
	}
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename, World& world,
							 AssetManager& assetManager) {
	world.getPhysicsSystem().clearStaticObstacles();

	TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

	initializeLayers(tmxMap.GetHeight() * tmxMap.GetTileHeight());

	MapData mapData;

	for (const auto& tileset : tmxMap.GetTilesets()) {
		TilesetData tilesetData = getTilesetData(tileset);
		mapData.tilesets.push_back(tilesetData);
	}

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

				tileData.scroll = tileLayerData.scroll;

				tileLayerData.tiles.push_back(tileData);
			}
		}

		mapData.tileLayers.push_back(tileLayerData);

		++layerIndex;
	}

	processObjects(tmxMap, world, mapData);

	std::vector<MapMesh> mapMeshes;
	buildMapMeshes(mapData, assetManager, mapMeshes);

	MapModel mapModel(mapMeshes);

	std::shared_ptr<Map> map =
		std::make_shared<Map>(
			tmxMap.GetWidth(), tmxMap.GetHeight(),
			tmxMap.GetTileWidth(), tmxMap.GetTileHeight(),
			mapModel);

	map->setAmbientLights(mapData.ambientLights);
	map->setPlayerStarts(mapData.playerStarts);

	return map;
}
}