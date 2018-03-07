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
	bool entityIsAlive(const Entity& entity) const;
	int getComponentIndexByEntity(const Entity& entity) const;
	const Entity& getEntityByComponentIndex(const int index) const;

public:
	EntitySystem(EntityManager& entityManager, unsigned int maxComponents);
	void addComponent(const Entity& entity);
	void garbageCollect();
	std::vector<Entity> getEntities() const;
	EntityManager& getEntityManager();
	bool hasComponent(const Entity& entity) const;
	const size_t maxComponents;
	void removeComponent(const Entity& entity);
};
}