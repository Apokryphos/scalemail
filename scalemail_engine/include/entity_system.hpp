#pragma once

#include "entity.hpp"
#include <unordered_map>

namespace ScaleMail
{
class EntityManager;

class EntitySystem
{
protected:
	size_t mComponentCount;

	EntityManager& mEntityManager;
	std::unordered_map<Entity, int> mComponentIndicesByEntity;
	std::unordered_map<int, Entity> mEntitiesByComponentIndices;

	virtual void createComponent() = 0;
	virtual void destroyComponent(int index) = 0;
	const Entity& getEntityByComponentIndex(const int index) const;

public:
	EntitySystem(EntityManager& entityManager, int maxComponents);
	void addComponent(const Entity& entity);
	void garbageCollect();
	EntityManager& getEntityManager();
	bool hasComponent(const Entity& entity) const;
	const int maxComponents;
	void removeComponent(const Entity& entity);
};
}