#pragma once

#include "camera.hpp"
#include "entity_system.hpp"
#include <vector>

namespace ScaleMail
{
struct Entity;
class PhysicsSystem;

struct CameraComponent {
	CameraComponent(const int index) { this->index = index; }
	int index;
};

struct CameraComponentData {
	Camera camera;
	Entity targetEntity;
};

class CameraSystem : public EntitySystem
{
private:
	std::vector<CameraComponentData> mData;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	CameraSystem(EntityManager& entityManager, int maxComponents = 10000);
	CameraComponent getComponent(const Entity& entity) const;
	const Camera& getCamera(const CameraComponent& cmpnt) const;
	void update(PhysicsSystem& physicsSystem, float elapsedSeconds);
};
}