#include "entity.hpp"

namespace ScaleMail
{
const unsigned ENTITY_INDEX_BITS = 22;
const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const unsigned ENTITY_GENERATION_BITS = 8;
const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

//	============================================================================
Entity makeEntity(const unsigned index, const unsigned generation) {
	return { (generation << ENTITY_INDEX_BITS) | index };
}

//	============================================================================
unsigned Entity::index() const {
	return this->id & ENTITY_INDEX_MASK;
}

//	============================================================================
unsigned Entity::generation() const {
	return (this->id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK;
}
}

//	============================================================================
std::size_t std::hash<ScaleMail::Entity>::operator()(
	const ScaleMail::Entity& entity) const {
	return std::hash<unsigned>()(entity.id);
}