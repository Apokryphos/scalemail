#pragma once

#include "ai/ai_node.hpp"
#include "team_alignment.hpp"
#include <string>

namespace ScaleMail
{
class TargetRangeAiNode : public AiNode
{
	TeamAlignment mTargetTeamAlignment;
	float mRange;
	std::string mTargetValueName;

public:
	TargetRangeAiNode(Entity& entity, AiTree* parentTree = nullptr,
					  float range = 64.0f,
					  TeamAlignment targetTeamAlignment = TeamAlignment::FOE,
					  const std::string& targetValueName = "TARGET");
	virtual AiNodeStatus execute(World& world, float elapsedSeconds) override;
	void setRange(float range);
	void setTargetTeamAlignment(TeamAlignment teamAlignment);
	void setTargetValueName(const std::string& name);
};
}