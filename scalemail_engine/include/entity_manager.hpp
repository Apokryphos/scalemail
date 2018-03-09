#pragma once

#include "entity.hpp"
#include <deque>
#include <vector>

namespace ScaleMail
{
class EntityManager
{
	const unsigned MINIMUM_FREE_INDICES = 1024;

	std::deque<unsigned> mFreeIndices;
	std::vector<unsigned> mGenerations;

public:
	EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	Entity createEntity();
	void destroyEntity(const Entity& entity);
	bool isAlive(const Entity& entity) const;
};
}