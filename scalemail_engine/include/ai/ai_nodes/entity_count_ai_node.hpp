#pragma once

#include "ai/ai_node.hpp"
#include <string>

namespace ScaleMail
{
class EntityCountAiNode : public AiNode
{
public:
enum class CompareType
{
	EQUAL,
	GREATER_THAN,
	LESS_THAN,
	GREATER_THAN_OR_EQUAL,
	LESS_THAN_OR_EQUAL,
	RANGE,
};

private:
	CompareType mCompareType;
	int mCountMin;
	int mCountMax;
	std::string mValueName;

public:
	EntityCountAiNode(Entity& entity, AiTree* parentTree,
					  std::string valueName = "TARGET");
	virtual AiNodeStatus execute(World& world) override;
	void setEqualTo(const int value);
	void setGreaterThanOrEqualTo(const int value);
	void setRange(const int min, const int max);
	void setValueName(const std::string& name);
};
}