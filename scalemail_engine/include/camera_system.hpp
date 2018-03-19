#pragma once

#include "camera.hpp"
#include "entity_system.hpp"
#include "path.hpp"
#include <glm/vec2.hpp>
#include <vector>

namespace ScaleMail
{
struct Entity;
class World;

struct CameraComponent {
	CameraComponent(const int index) { this->index = index; }
	int index;
};

enum class CameraMode {
	FIXED,
	FOLLOW_ENTITY,
	FOLLOW_PATH,
};

struct CameraComponentData {
	CameraMode mode;
	Camera camera;
	Entity targetEntity;
	std::vector<Rectangle> bounds;
	std::vector<Path> paths;
};

class CameraSystem : public EntitySystem
{
private:
	//	Camera defaults
	int mWidth;
	int mHeight;
	float mZoom;

	std::vector<CameraComponentData> mData;

	World& mWorld;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;
	void initializeFollowEntity(const CameraComponent& cmpnt);
	void initializeFollowPath(const CameraComponent& cmpnt);
	void updateBounds(const glm::vec2& position, CameraComponentData& data);
	void updateFixed(World& world, const Entity& cameraEntity,
					 CameraComponentData& data);
	void updateFollowEntity(World& world, const Entity& cameraEntity,
							CameraComponentData& data);
	void updateFollowPath(World& world, const Entity& cameraEntity,
						  CameraComponentData& data);

public:
	CameraSystem(World& world, EntityManager& entityManager, int maxComponents = 10000);
	void followEntity(const CameraComponent& cmpnt, const Entity& targetEntity);
	CameraComponent getComponent(const Entity& entity) const;
	Camera& getCamera(const CameraComponent& cmpnt);
	const Camera& getCamera(const CameraComponent& cmpnt) const;
	void initialize(World& world);
	void setBounds(const CameraComponent& cmpnt, const std::vector<Rectangle>& bounds);
	void setCameraDefaults(const int width, const int  height, const float zoom);
	void setMode(const CameraComponent& cmpnt, const CameraMode mode);
	void setPaths(const CameraComponent& cmpnt, const std::vector<Path>& paths);
	void update(float elapsedSeconds);
};
}