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
	if (object.getPropertySet().getBoolValue("Buried", false)) {
		buryEntity(entity, world.getBurySystem(), true,
			world.getRandom().nextFloat(4.0f, 5.0f), true);
	}
}

//  ============================================================================
//	Get the tileset image source name without the path and extension
static std::string getTilesetImageName(const TmxMapLib::Tileset& tileset) {
	std::string filename = tileset.getImage().getSource();

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
	tilesetData.firstGid = tileset.getFirstGid();
	tilesetData.tileCount = tileset.getTileCount();
	tilesetData.tileWidth = tileset.getTileWidth();
	tilesetData.tileHeight = tileset.getTileHeight();
	tilesetData.textureName = getTilesetImageName(tileset);
	return tilesetData;
}

//  ============================================================================
static glm::vec2 getTileObjectPosition(const TmxMapLib::Object& object) {
	return glm::vec2(
		object.getX() + object.getWidth() * 0.5f,
		object.getY() - object.getHeight() * 0.5f);
}

//  ============================================================================
static bool getTilesetAnimation(const TmxMapLib::Map& tmxMap, const int gid,
								int& nextFrameGid) {
	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->getTile(gid - tileset->getFirstGid());

	if (tilesetTile) {
		const TmxMapLib::Animation& animation = tilesetTile->getAnimation();

		if(animation.getFrameCount() > 0) {
			int nextFrameId = animation.getFrame(1).getTileId();

			nextFrameGid = nextFrameId + tileset->getFirstGid();

			return true;
		}
	}

	return false;
}

//  ============================================================================
static void loadTileDataProperties(TileData& tileData,
								   const TmxMapLib::TilesetTile& tilesetTile) {
	const TmxMapLib::PropertySet& propertySet = tilesetTile.getPropertySet();

	tileData.alpha =
		propertySet.getBoolValue("AlphaTest", false);

	tileData.wallFace =
		propertySet.getBoolValue("WallFace", false);

	tileData.wallTop =
		propertySet.getBoolValue("WallTop", false);

	if (propertySet.hasProperty("AliasTileset") ||
		propertySet.hasProperty("AliasTilesetId")) {
		throw std::runtime_error("AliasTileset and AliasTilesetId properties are deprecated.");
	}
}

//  ============================================================================
static TileData loadTileData(const TmxMapLib::Map& tmxMap,
							 const TmxMapLib::Tile& tile) {
	int gid = tile.getGid();

	TileData tileData = {};
	tileData.gid = gid;
	tileData.x = tile.getX();
	tileData.y = tile.getY();
	tileData.flipHorz = tile.getFlipHorizontally();
	tileData.flipVert = tile.getFlipVertically();
	tileData.flipDiag = tile.getFlipDiagonally();

	//	Load properties from tileset tile
	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(gid);

	//	Set initial animation
	tileData.nextFrameGid = tileData.gid;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrameGid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->getTile(tileData.gid - tileset->getFirstGid());

	//	Return early if there's no matching tileset tile
	if (tilesetTile == nullptr) {
		return tileData;
	}

	const TmxMapLib::PropertySet& propertySet = tilesetTile->getPropertySet();

	//	Is this an odd/even tile (like a checkerboard)?
	const bool odd = (tileData.x + tileData.y) % 2;

	//	Set tileset ID to alternate odd/even ID if it is specified
	if (odd && propertySet.hasProperty("OddTilesetId")) {
		gid = propertySet.getIntValue("OddTilesetId", gid);
		gid += tileset->getFirstGid();
	} else if (!odd && propertySet.hasProperty("EvenTilesetId")) {
		gid = propertySet.getIntValue("EvenTilesetId", gid);
		gid += tileset->getFirstGid();
	}

	//	Update to actual tileset ID
	tileData.gid = gid;

	//	Update animation to actual tileset ID
	tileData.nextFrameGid = gid;
	tileData.animated = getTilesetAnimation(tmxMap, gid, tileData.nextFrameGid);

	//	Get the actual tileset tile to use for properties
	const TmxMapLib::TilesetTile* finalTilesetTile =
		tileset->getTile(gid - tileset->getFirstGid());

	if (finalTilesetTile != nullptr) {
		loadTileDataProperties(tileData, *finalTilesetTile);
	}

	return tileData;
}

//  ============================================================================
static float getDecal(const TmxMapLib::Map& tmxMap, const int gid) {
	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(gid);

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->getTile(gid - tileset->getFirstGid());

	if (tilesetTile) {
		return tilesetTile->getPropertySet().getBoolValue("Decal", false);
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
			mapMesh.textureId == tileset.getTexture().id;
	});

	if (itr != mapMeshes.end()) {
		return *itr;
	}

	MapMesh mapMesh = {};
	mapMesh.alpha = tileData.alpha;
	mapMesh.animated = tileData.animated;
	mapMesh.frame = frame;
	mapMesh.scroll = tileData.scroll;
	mapMesh.textureId = tileset.getTexture().id;

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
	auto const tile = object.getTile();

	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(tile->getGid());

	if (tileset == nullptr) {
		std::cout << "Invalid actor object in TMX map: no matching tileset." << std::endl;
		return;
	}

	const int tilesetId = tile->getGid() - tileset->getFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->getTile(tilesetId);

	const TmxMapLib::PropertySet& propertySet = tilesetTile->getPropertySet();

	if (tilesetTile == nullptr) {
		std::cout << "Invalid actor object in TMX map: no matching tileset tile." << std::endl;
		return;
	}

	const TmxMapLib::Property* actorIndexProperty =
		tilesetTile->getPropertySet().getProperty("ActorIndex");

	if (actorIndexProperty == nullptr) {
		std::cout << "Invalid actor object in TMX map: no tileset tile with ActorIndex property." << std::endl;
		return;
	}

	int actorIndex = actorIndexProperty->getIntValue(0);

	const TmxMapLib::Property* facingProperty =
		tilesetTile->getPropertySet().getProperty("Facing");

	Direction facing = Direction::SOUTH;

	if (facingProperty != nullptr) {
		const std::string value = facingProperty->getValue();
		facing = stringToDirection(value);
	}

	const std::string aiName = toLowercase(propertySet.getValue("Ai", ""));
	const std::string prefabName = toLowercase(propertySet.getValue("Prefab", ""));

	const glm::vec2 position = getTileObjectPosition(object);
	const glm::vec2 size(object.getWidth(), object.getHeight());

	Entity entity = createVillainActor(world, position, size, actorIndex,
									   facing, object.getName(), prefabName,
									   aiName);

	buryEntity(object, entity, world);
}

//  ============================================================================
static void processActorCollisionObject(World& world,
										const TmxMapLib::Object& object) {
	const float x = object.getX();
	const float y = object.getY();
	const float width = object.getWidth();
	const float height = object.getHeight();

	world.getPhysicsSystem().addStaticActorObstacle(x, y, width, height);

	world.getAiSystem().addStaticActorObstacle(x, y, width, height);
}

//  ============================================================================
static void processAmbientLightObject(const TmxMapLib::Object& object,
									  const Polygon& polygon,
									  MapData& mapData) {
	glm::vec4 color =
		hexToVec4(object.getPropertySet().getValue("Color", "#FFFFFF"));

	mapData.ambientLights.push_back({color, polygon});
}

//  ============================================================================
static void processAmbientLightRectangle(const TmxMapLib::Object& object,
											   MapData& mapData) {
	const float x = object.getX();
	const float y = object.getY();
	const float width = object.getWidth();
	const float height = object.getHeight();

	Polygon polygon(x, y, width, height);

	processAmbientLightObject(object, polygon, mapData);
}

//  ============================================================================
static void processAmbientLightPolygon(const TmxMapLib::Object& object,
											 MapData& mapData) {
	const float x = object.getX();
	const float y = object.getY();

	std::vector<glm::vec2> points;
	for (const auto& p : object.getPoints()) {
		points.emplace_back(x + p.x, y + p.y);
	}

	Polygon polygon(points);

	processAmbientLightObject(object, polygon, mapData);
}

//  ============================================================================
static void processCameraBoundsObject(World& world,
									  const Polygon& polygon,
									  const TmxMapLib::Object& object) {
	const bool visited = object.getPropertySet().getBoolValue("Visited", false);

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.addBounds(Bounds(polygon), visited);
}

//  ============================================================================
static void processCameraBoundsRectangle(World& world,
										 const TmxMapLib::Object& object) {
	const float x = object.getX();
	const float y = object.getY();
	const float width = object.getWidth();
	const float height = object.getHeight();

	processCameraBoundsObject(world, Polygon(x, y, width, height), object);
}

//  ============================================================================
static void processCameraBoundsPolygon(World& world,
									   const TmxMapLib::Object& object) {
	const float x = object.getX();
	const float y = object.getY();

	std::vector<glm::vec2> points;
	for (const auto& p : object.getPoints()) {
		points.emplace_back(x + p.x, y + p.y);
	}

	processCameraBoundsObject(world, Polygon(points), object);
}

//  ============================================================================
static void processCameraPathObject(World& world,
									const TmxMapLib::Object& object) {
	const float x = object.getX();
	const float y = object.getY();

	Path path = {};

	for (const auto& point : object.getPoints()) {
		path.points.push_back(glm::vec2(x + point.x, y + point.y));
	}

	CameraSystem& cameraSystem = world.getCameraSystem();
	cameraSystem.addPath(object.getName(), path);
}

//  ============================================================================
static void processCollisionObject(World& world,
							  	   const TmxMapLib::Object& object) {
	const float x = object.getX();
	const float y = object.getY();
	const float width = object.getWidth();
	const float height = object.getHeight();

	world.getPhysicsSystem().addStaticObstacle(x, y, width, height);

	world.getAiSystem().addStaticObstacle(x, y, width, height);
}

//  ============================================================================
static void processDoorObject(World& world,
							  const TmxMapLib::Object& object,
							  const TmxMapLib::Map& tmxMap) {
	const glm::vec2 position = getTileObjectPosition(object);

	const TmxMapLib::Tile* tile = object.getTile();

	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(tile->getGid());

	const int tilesetId = tile->getGid() - tileset->getFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile =
		tileset->getTile(tilesetId);

	bool open = false;

	if (tilesetTile != nullptr) {
		open = tilesetTile->getPropertySet().getBoolValue("DoorOpen", false);

		int openTilesetId = tilesetId;
		int closedTilesetId = tilesetId;

		if (open) {
			closedTilesetId = tilesetTile->getPropertySet().getIntValue(
				"DoorClosedId", tilesetId);
		} else {
			openTilesetId = tilesetTile->getPropertySet().getIntValue(
				"DoorOpenId", tilesetId);
		}

		if (openTilesetId == closedTilesetId) {
			std::cout <<
				"Door object does not have correct " <<
				(open ? "DoorClosedId" : "DoorOpenId") <<
				" property." << std::endl;
			return;
		}

		if (tilesetTile->getPropertySet().getBoolValue("DoorChild", false)) {
			createDoor(world, position, openTilesetId, closedTilesetId,
					   openTilesetId + 1, closedTilesetId + 1, open,
					   object.getName());
		} else {
			createDoor(world, position, openTilesetId, closedTilesetId,
					   open, object.getName());
		}

	} else {
		std::cout << "Door object missing tileset properties.";
	}
}

//  ============================================================================
static void processItemObject(World& world,
							   const TmxMapLib::Object& object,
							   const TmxMapLib::Map& tmxMap) {
	auto const tile = object.getTile();

	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(tile->getGid());

	if (tileset == nullptr) {
		std::cout << "Invalid item object in TMX map: no matching tileset." << std::endl;
		return;
	}

	const int tilesetId = tile->getGid() - tileset->getFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->getTile(tilesetId);

	const TmxMapLib::PropertySet& propertySet = tilesetTile->getPropertySet();

	if (tilesetTile == nullptr) {
		std::cout << "Invalid item object in TMX map: no matching tileset tile." << std::endl;
		return;
	}

	const std::string prefabName = toLowercase(propertySet.getValue("Prefab", ""));

	const glm::vec2 size(object.getWidth(), object.getHeight());

	const glm::vec2 position = getTileObjectPosition(object);

	const std::string name = object.getName();

	Entity entity =
		createLoot(world, position, size, tilesetId, name, prefabName);

	buryEntity(object, entity, world);
}

//  ============================================================================
static void processLightObject(World& world,
							   const TmxMapLib::Object& object) {
	//  Scale light size by constant...lights are too small
	glm::vec2 lightSize =
		glm::vec2(object.getWidth(), object.getHeight()) * 2.5f;

	const auto& propertySet = object.getPropertySet();

	std::string hex = propertySet.getValue("LightColor", "#FFFFFFFF");
	glm::vec4 lightColor = hexToVec4(hex);

	float lightPulse = propertySet.getFloatValue("LightPulse", 0);

	float lightPulseSize = propertySet.getFloatValue("LightPulseSize", 0);

	const float x = object.getX() + object.getWidth() * 0.5f;
	const float y = object.getY() + object.getHeight() * 0.5f;

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
	auto const tile = object.getTile();

	const int gid = tile->getGid();

	const auto& tileset = tmxMap.getTilesetByGid(gid);

	const int tilesetId = gid - tileset->getFirstGid();

	bool decal = getDecal(tmxMap, gid);

	int nextFrameGid = gid;
	getTilesetAnimation(tmxMap, gid, nextFrameGid);

	const int nextFrameTilesetId = nextFrameGid - tileset->getFirstGid();

	const glm::vec2 position = getTileObjectPosition(object);

	createProp(world, position, tilesetId, nextFrameTilesetId, decal);
}

//  ============================================================================
static bool processPlayerStartObject(const TmxMapLib::Object& object,
									 const TmxMapLib::Map& tmxMap,
									 PlayerStart& playerStart) {
	auto const tile = object.getTile();

	const TmxMapLib::Tileset* tileset = tmxMap.getTilesetByGid(tile->getGid());

	if (tileset == nullptr) {
		std::cout << "Invalid player start object in TMX map: no matching tileset." << std::endl;
		return false;
	}

	int tilesetId = tile->getGid() - tileset->getFirstGid();

	const TmxMapLib::TilesetTile* tilesetTile = tileset->getTile(tilesetId);

	if (tilesetTile == nullptr) {
		std::cout << "Invalid player start object in TMX map: no matching tileset tile." << std::endl;
		return false;
	}

	const TmxMapLib::Property* actorIndexProperty =
		tilesetTile->getPropertySet().getProperty("ActorIndex");

	if (actorIndexProperty == nullptr) {
		std::cout << "Invalid player start object in TMX map: no tileset tile with ActorIndex property." << std::endl;
		return false;
	}

	int actorIndex = actorIndexProperty->getIntValue(0);

	const TmxMapLib::Property* facingProperty =
		tilesetTile->getPropertySet().getProperty("Facing");

	Direction facing = Direction::SOUTH;

	if (facingProperty != nullptr) {
		const std::string value = facingProperty->getValue();
		facing = stringToDirection(value);
	}

	playerStart.position = getTileObjectPosition(object);
	playerStart.actorIndex = actorIndex;
	playerStart.facing = facing;
	return true;
}

//  ============================================================================
static void processTorchObject(World& world,
							   const TmxMapLib::Object& object,
							   const TmxMapLib::Map& tmxMap) {
	const auto tile = object.getTile();

	const int gid = tile->getGid();

	const auto tileset = tmxMap.getTilesetByGid(gid);

	const int tilesetId = gid - tileset->getFirstGid();

	int nextFrameGid = gid;
	getTilesetAnimation(tmxMap, gid, nextFrameGid);

	const int nextFrameTilesetId = nextFrameGid - tileset->getFirstGid();

	const glm::vec2 position = getTileObjectPosition(object);

	createTorch(world, position, tilesetId, nextFrameTilesetId);
}

//  ============================================================================
static void processTriggerObject(World& world,
								 const TmxMapLib::Object& object) {
	if (!object.getPropertySet().hasProperty("TargetName")) {
		std::cout << "Trigger object is missing TargetName property." << std::endl;
		return;
	}

	const glm::vec2 position(object.getX(), object.getY());
	const glm::vec2 size(object.getWidth(), object.getHeight());

	const std::string targetName =
		object.getPropertySet().getValue("TargetName", "");

	createTrigger(world, position, size, targetName);
}

//  ============================================================================
static void processObject(World& world,
						  const TmxMapLib::Object& object,
						  const TmxMapLib::Map& tmxMap,
						  MapData& mapData) {
	const std::string type = toLowercase(object.getType());

	if (object.getObjectType() == TmxMapLib::ObjectType::Ellipse) {
		if (type == "light") {
			processLightObject(world, object);
		} else {
			std::cout << "Unrecognized ellipse map object." << std::endl;
		}
	}

	if (object.getObjectType() == TmxMapLib::ObjectType::Tile) {
		if (object.getTile() == nullptr) {
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
	} else if (object.getObjectType() == TmxMapLib::ObjectType::Basic) {
		if (type == "collision") {
			processCollisionObject(world, object);
		} else if (type == "actorcollision") {
			processActorCollisionObject(world, object);
		} else if (type == "ambientlight") {
			processAmbientLightRectangle(object, mapData);
		} else if (type == "camerabounds") {
			processCameraBoundsRectangle(world, object);
		} else if (type == "trigger") {
			processTriggerObject(world, object);
		}
	} else if (object.getObjectType() == TmxMapLib::ObjectType::Polyline) {
		if (type == "camerapath") {
			processCameraPathObject(world, object);
		}
	} else if (object.getObjectType() == TmxMapLib::ObjectType::Polygon) {
		if (type == "ambientlight") {
			processAmbientLightPolygon(object, mapData);
		} else if (type == "camerabounds") {
			processCameraBoundsPolygon(world, object);
		}
	}
}

//  ============================================================================
static void processObjects(const TmxMapLib::Map tmxMap, World& world,
						   MapData& mapData) {
	for (const auto& objectGroup : tmxMap.getObjectGroups()) {
		std::vector<Rectangle> cameraBounds;
		std::vector<Path> cameraPaths;

		for (const auto& object : objectGroup.getObjects()) {
			processObject(world, object, tmxMap, mapData);
		}
	}
}

//  ============================================================================
std::shared_ptr<Map> loadMap(const std::string filename, World& world,
							 AssetManager& assetManager) {
	world.getPhysicsSystem().clearStaticObstacles();

	TmxMapLib::Map tmxMap = TmxMapLib::Map(filename);

	initializeLayers(tmxMap.getHeight() * tmxMap.getTileHeight());

	MapData mapData;

	for (const auto& tileset : tmxMap.getTilesets()) {
		TilesetData tilesetData = getTilesetData(tileset);
		mapData.tilesets.push_back(tilesetData);
	}

	int layerIndex = 0;
	for (const auto& tileLayer : tmxMap.getTileLayers()) {
		TileLayerData tileLayerData;

		tileLayerData.layerZ =
			tileLayer.getPropertySet().getIntValue("LayerZ", layerIndex);

		tileLayerData.scroll =
			tileLayer.getPropertySet().getBoolValue("LayerScroll", false);

		for (int tileY = 0; tileY < tileLayer.getHeight(); ++tileY) {
			for (int tileX = 0; tileX < tileLayer.getWidth(); ++tileX) {
				const TmxMapLib::Tile* tile = tileLayer.getTile(tileX, tileY);

				if (tile->getGid() == 0) {
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
			tmxMap.getWidth(), tmxMap.getHeight(),
			tmxMap.getTileWidth(), tmxMap.getTileHeight(),
			mapModel);

	map->setAmbientLights(mapData.ambientLights);
	map->setPlayerStarts(mapData.playerStarts);

	return map;
}
}