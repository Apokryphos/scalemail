#include "team_system.hpp"
#include "vector_util.hpp"
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
static TeamComponent makeComponent(const int index) {
	return TeamComponent(index);
}

//	============================================================================
TeamSystem::TeamSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mTeam.reserve(maxComponents);
	mPlayerEntities.reserve(maxComponents);
	mVillainEntities.reserve(maxComponents);
}

//	============================================================================
void TeamSystem::createComponent() {
	const size_t cmpntIndex = mTeam.size();
	const Entity& entity = this->getEntityByComponentIndex(cmpntIndex);

	mTeam.emplace_back(Team::VILLAIN);

	mVillainEntities.push_back(entity);
}

//	============================================================================
void TeamSystem::destroyComponent(int index) {
	const Entity& entity = this->getEntityByComponentIndex(index);

	mPlayerEntities.erase(
		std::remove_if(mPlayerEntities.begin(), mPlayerEntities.end(),
		[entity](const Entity& e) {
			return e.id == entity.id;
		}),
		mPlayerEntities.end());

	mVillainEntities.erase(
		std::remove_if(mVillainEntities.begin(), mVillainEntities.end(),
			[entity](const Entity& e) {
				return e.id == entity.id;
			}),
		mVillainEntities.end());

	swapWithLastElementAndRemove(mTeam, index);
}

//	============================================================================
void TeamSystem::getAlliesByTeam(const Team team,
								 std::vector<Entity>& entities) {
	switch (team) {
		case Team::VILLAIN:
			this->getEntitiesByTeam(Team::VILLAIN, entities);
			break;

		case Team::PLAYER:
			this->getEntitiesByTeam(Team::PLAYER, entities);
			break;
	}
}

//	============================================================================
TeamComponent TeamSystem::getComponent(const Entity& entity) const {
	return makeComponent(mComponentIndicesByEntity.at(entity));
}

//	============================================================================
void TeamSystem::getEntitiesByTeam(const Team team,
								   std::vector<Entity>& entities) {
	switch (team) {
		case Team::PLAYER:
			entities.insert(entities.begin(), mPlayerEntities.begin(),
							mPlayerEntities.end());
			break;
		case Team::VILLAIN:
			entities.insert(entities.begin(), mVillainEntities.begin(),
							mVillainEntities.end());
			break;
	}
}

//	============================================================================
void TeamSystem::getEntitiesByTeamAlignment(const Team team,
											const TeamAlignment teamAlignment,
											std::vector<Entity>& entities) {
	switch (teamAlignment) {
		case TeamAlignment::ANY:
			entities.insert(entities.begin(), mPlayerEntities.begin(),
							mPlayerEntities.end());
			entities.insert(entities.begin(), mVillainEntities.begin(),
							mVillainEntities.end());
			break;

		case TeamAlignment::ALLY:
			return this->getAlliesByTeam(team, entities);

		case TeamAlignment::FOE:
			return this->getFoesByTeam(team, entities);
	}
}

//	============================================================================
void TeamSystem::getEntitiesByTeamAlignment(const TeamComponent& cmpnt,
											const TeamAlignment teamAlignment,
											std::vector<Entity>& entities) {
	return this->getEntitiesByTeamAlignment(
		mTeam[cmpnt.index], teamAlignment, entities);
}

//	============================================================================
void TeamSystem::getFoes(const TeamComponent& cmpnt,
						 std::vector<Entity>& entities) {
	return this->getFoesByTeam(mTeam[cmpnt.index], entities);
}

//	============================================================================
void TeamSystem::getFoesByTeam(const Team team,
							   std::vector<Entity>& entities) {
	switch (team) {
		case Team::PLAYER:
			this->getEntitiesByTeam(Team::VILLAIN, entities);
			break;

		case Team::VILLAIN:
			this->getEntitiesByTeam(Team::PLAYER, entities);
			break;
	}
}

//	============================================================================
Team TeamSystem::getTeam(const TeamComponent& cmpnt) const {
	return mTeam[cmpnt.index];
}

//	============================================================================
void TeamSystem::setTeam(const TeamComponent& cmpnt, const Team team) {
	if (mTeam[cmpnt.index] == team) {
		return;
	}

	const Entity& entity = this->getEntityByComponentIndex(cmpnt.index);

	mPlayerEntities.erase(
		std::remove_if(mPlayerEntities.begin(), mPlayerEntities.end(),
		[entity](const Entity& e) {
			return e.id == entity.id;
		}),
		mPlayerEntities.end());

	mVillainEntities.erase(
		std::remove_if(mVillainEntities.begin(), mVillainEntities.end(),
			[entity](const Entity& e) {
				return e.id == entity.id;
			}),
		mVillainEntities.end());

	mTeam[cmpnt.index] = team;

	switch (team) {
		case Team::PLAYER:
			mPlayerEntities.push_back(entity);
			break;
		case Team::VILLAIN:
			mVillainEntities.push_back(entity);
			break;
	}
}
}