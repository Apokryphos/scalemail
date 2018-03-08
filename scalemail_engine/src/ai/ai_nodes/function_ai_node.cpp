#include "ai/ai_nodes/function_ai_node.hpp"

namespace ScaleMail
{
//	============================================================================
FunctionAiNode::FunctionAiNode(Entity& entity, AiTree* parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus FunctionAiNode::execute(World& world) {
	if (mFunction) {
		return mFunction(*this, world);
	}

	//	Success if no function is set
	return AiNodeStatus::SUCCESS;
}

//	============================================================================
void FunctionAiNode::setFunction(
	std::function<AiNodeStatus(AiNode&, World&)> func) {
	mFunction = func;
}
}