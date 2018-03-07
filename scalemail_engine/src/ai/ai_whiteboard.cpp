#include "ai/ai_whiteboard.hpp"
#include <cassert>

namespace ScaleMail
{
static const std::vector<Entity> emptyEntities;

//	============================================================================
void AiWhiteboard::addEntity(const std::string& name, const Entity& entity) {
	mEntities[name].push_back(entity);
}

//	============================================================================
void AiWhiteboard::clearEntities(const std::string& name) {
	mEntities[name].resize(0);
}

//	============================================================================
const std::vector<Entity>& AiWhiteboard::getEntities(const std::string& name) const {
	const auto& pair = mEntities.find(name);

	if (pair == mEntities.end()) {
		assert(emptyEntities.size() == 0);
		return emptyEntities;
	}

	return pair->second;
}

//	============================================================================
float AiWhiteboard::getFloatValue(const std::string& name,
								  float defaultValue) const {
	const auto& pair = mFloatValues.find(name);

	if (pair == mFloatValues.end()) {
		return defaultValue;
	}

	return pair->second;
}

//	============================================================================
void AiWhiteboard::setFloatValue(const std::string& name, float value) {
	mFloatValues[name] = value;
}
}