#pragma once

#include "entity_system.hpp"
#include "team.hpp"
#include <vector>

namespace ScaleMail
{
struct TeamComponent {
	TeamComponent(const int index) { this->index = index; }
	int index;
};

class TeamSystem : public EntitySystem
{
	std::vector<Team> mTeam;

	std::vector<Entity> mPlayerEntities;
	std::vector<Entity> mVillainEntities;

	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	TeamSystem(EntityManager& entityManager, int maxComponents = 1000);
	TeamComponent getComponent(const Entity& entity) const;
	void getEntitiesByTeam(const Team team, std::vector<Entity>& entities);
	void getFoes(const TeamComponent& cmpnt, std::vector<Entity>& entities);
	void getFoesByTeam(const Team team, std::vector<Entity>& entities);
	Team getTeam(const TeamComponent& cmpnt) const;
	void setTeam(const TeamComponent& cmpnt, const Team team);
};
}