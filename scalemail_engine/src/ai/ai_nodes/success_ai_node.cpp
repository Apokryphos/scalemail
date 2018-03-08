#include "ai/ai_nodes/success_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
SuccessAiNode::SuccessAiNode(Entity& entity, AiTree* parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus SuccessAiNode::execute([[maybe_unused]]World& world,
									[[maybe_unused]]float elapsedSeconds) {
	return AiNodeStatus::SUCCESS;
}
}