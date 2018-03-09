#pragma once

#include "ai/ai_node.hpp"
#include "team_alignment.hpp"
#include <string>

enum class TargetType
{
	ACTOR,
	LOOT,
};

namespace ScaleMail
{
class TargetRangeAiNode : public AiNode
{
private:
	TargetType mTargetType;
	TeamAlignment mTargetTeamAlignment;
	float mRange;
	std::string mTargetValueName;

public:
	TargetRangeAiNode(Entity& entity, AiTree& parentTree,
					  float range = 64.0f,
					  TeamAlignment targetTeamAlignment = TeamAlignment::FOE,
					  const std::string& targetValueName = "TARGET");
	virtual AiNodeStatus execute(World& world) override;
	void setRange(float range);
	void setTargetTeamAlignment(TeamAlignment teamAlignment);
	void setTargetType(const TargetType targetType);
	void setTargetValueName(const std::string& name);
};
}