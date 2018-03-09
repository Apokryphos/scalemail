#include "ai/ai_nodes/failure_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
FailureAiNode::FailureAiNode(Entity& entity, AiTree& parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus FailureAiNode::execute([[maybe_unused]]World& world) {
	return AiNodeStatus::FAILURE;
}
}