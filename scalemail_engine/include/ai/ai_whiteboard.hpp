#pragma once

#include "entity.hpp"
#include <string>
#include <vector>

namespace ScaleMail
{
class AiNode;
class World;

class AiWhiteboard
{
	std::unordered_map<std::string, std::vector<Entity>> mEntities;
	std::unordered_map<std::string, float> mFloatValues;

public:
	AiWhiteboard() = default;
	AiWhiteboard(const AiWhiteboard&) = delete;
	AiWhiteboard& operator=(const AiWhiteboard&) = delete;
	void addEntity(const std::string& name, const Entity& entity);
	void clearEntities(const std::string& name);
	const std::vector<Entity>& getEntities(const std::string& name) const;
	int getEntityCount(const std::string& name) const;
	float getFloatValue(const std::string& name, float defaultValue) const;
	void removeDeadEntities(World& world);
	void setFloatValue(const std::string& name, float value);
};
}