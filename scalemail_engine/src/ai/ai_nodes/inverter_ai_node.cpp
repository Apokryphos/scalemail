#include "ai/ai_nodes/inverter_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
InverterAiNode::InverterAiNode(Entity& entity, AiTree* parentTree)
: DecoratorAiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus InverterAiNode::execute(World& world) {
	if (!this->hasChildNode()) {
		return AiNodeStatus::FAILURE;
	}

	AiNodeStatus status =
		this->getChildNode()->execute(world);

	switch (status) {
		case AiNodeStatus::FAILURE:
			return AiNodeStatus::SUCCESS;

		case AiNodeStatus::SUCCESS:
			return AiNodeStatus::FAILURE;

		case AiNodeStatus::RUNNING:
			return AiNodeStatus::RUNNING;

		default:
		throw std::runtime_error("Case not implemented.");
	}
}
}