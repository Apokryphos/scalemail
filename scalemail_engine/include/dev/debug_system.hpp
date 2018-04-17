#pragma once

#include "entity_system.hpp"
#include <string>
#include <vector>

namespace ScaleMail
{
class World;

struct DebugComponent {
	DebugComponent(const int index) { this->index = index; }
	int index;
};

struct DebugComponentData {
	bool selected;
	Entity entity;
	std::string idString;
};

class DebugSystem : public EntitySystem
{
	World& mWorld;

	std::vector<DebugComponentData> mData;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	DebugSystem(World& world, EntityManager& entityManager,
				int maxComponents = 1000);
	void clearSelected();
	void drawDebug(std::vector<float>& lineVertexData);
	DebugComponent getComponent(const Entity& entity);
	const std::vector<DebugComponentData>& getComponentData() const;
	std::vector<Entity> getEntities() const;
	const std::string& getIdString(const DebugComponent& cmpnt) const;
	void setSelected(const DebugComponent& cmpnt, bool selected);
};
}