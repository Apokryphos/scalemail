#pragma once

#include <functional>

namespace ScaleMail
{
struct Entity
{
	unsigned id;

	// Entity(const unsigned index, const unsigned generation);
	unsigned index() const;
	unsigned generation() const;
};

Entity makeEntity(const unsigned index, const unsigned generation);

inline bool operator==(const Entity& entity1, const Entity& entity2) {
	return entity1.id == entity2.id;
}

inline bool operator!=(const Entity& entity1, const Entity& entity2) {
	return entity1.id != entity2.id;
}

inline bool operator<(const Entity& entity1, const Entity& entity2) {
	return entity1.id < entity2.id;
}
}

namespace std
{
template <>
class hash<ScaleMail::Entity>
{
public:
	size_t operator()(const ScaleMail::Entity& entity) const;
};
}