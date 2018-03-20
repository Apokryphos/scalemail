#pragma once

#include "camera.hpp"
#include "entity_system.hpp"
#include "path.hpp"
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>

namespace ScaleMail
{
struct Entity;
class World;

struct CameraComponent
{
	CameraComponent(const int index) { this->index = index; }
	int index;
};

struct CameraVisibility
{
	bool visited;
	float alpha;
	float alphaDirection;
	float alphaDuration;
	float alphaTicks;
	Rectangle bounds;
};

enum class CameraMode
{
	FIXED,
	FOLLOW_ENTITY,
	FOLLOW_PATH,
};

struct CameraComponentData
{
	CameraMode mode;
	Camera camera;
	Entity targetEntity;
	Path path;
	glm::vec2 pathStart;
	glm::vec2 pathEnd;
};

class CameraSystem : public EntitySystem
{
private:
	//	Camera defaults
	int mWidth;
	int mHeight;
	float mZoom;

	std::vector<CameraComponentData> mData;

	std::vector<CameraVisibility> mVisibility;
	std::unordered_map<std::string, Path> mPaths;

	World& mWorld;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;
	void initializeFixed(const CameraComponent& cmpnt);
	void initializeFollowEntity(const CameraComponent& cmpnt);
	void initializeFollowPath(const CameraComponent& cmpnt);
	void updateBounds(const glm::vec2& position, CameraComponentData& data);
	void updateFixed(const Entity& cameraEntity, CameraComponentData& data);
	void updateFollowEntity(const Entity& cameraEntity,
							CameraComponentData& data);
	void updateFollowPath(const Entity& cameraEntity,
						  CameraComponentData& data);
	void updateVisibility(const Camera& camera);

public:
	CameraSystem(World& world, EntityManager& entityManager, int maxComponents = 10000);
	void addBounds(const Rectangle& bounds, bool visited);
	void addPath(const std::string& name, const Path& path);
	void followEntity(const CameraComponent& cmpnt, const Entity& targetEntity);
	CameraComponent getComponent(const Entity& entity) const;
	Camera& getCamera(const CameraComponent& cmpnt);
	const Camera& getCamera(const CameraComponent& cmpnt) const;
	const std::vector<CameraVisibility> getVisibility() const;
	void setCameraDefaults(const int width, const int  height, const float zoom);
	void setMode(const CameraComponent& cmpnt, const CameraMode mode);
	void setPath(const CameraComponent& cmpnt, const std::string& pathName);
	void update(float elapsedSeconds);
};
}