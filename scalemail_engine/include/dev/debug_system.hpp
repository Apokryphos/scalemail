#pragma once

#include "entity_system.hpp"
#include <string>

namespace ScaleMail
{
struct DebugComponent {
	DebugComponent(const int index) { this->index = index; }
	int index;
};

struct DebugComponentData {
	Entity entity;
	std::string idString;
};

class DebugSystem : public EntitySystem
{
	std::vector<DebugComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	DebugSystem(EntityManager& entityManager, int maxComponents = 1000);
	DebugComponent getComponent(const Entity& entity);
	const std::vector<DebugComponentData>& getComponentData() const;
	std::vector<Entity> getEntities() const;
	const std::string& getIdString(const DebugComponent& cmpnt) const;
};
}