#include "ai/ai_nodes/failure_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
FailureAiNode::FailureAiNode(Entity& entity, AiTree* parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus FailureAiNode::execute([[maybe_unused]]World& world,
									[[maybe_unused]]float elapsedSeconds) {
	return AiNodeStatus::FAILURE;
}
}