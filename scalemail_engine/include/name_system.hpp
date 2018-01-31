#pragma once

#include "entity_system.hpp"
#include <string>
#include <vector>

namespace ScaleMail
{
struct NameComponent {
	NameComponent(const int index) { this->index = index; }
	int index;
};

class NameSystem : public EntitySystem
{
	std::vector<std::string> mName;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	NameSystem(EntityManager& entityManager, int maxComponents = 1000);
	NameComponent getComponent(const Entity& entity);
	std::vector<Entity> getEntitiesByName(const std::string name) const;
	std::string getName(const NameComponent& cmpnt) const;
  	void setName(const NameComponent& cmpnt, const std::string name);
};
}