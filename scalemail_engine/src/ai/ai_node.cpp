#include "ai/ai_node.hpp"
#include "ai/ai_tree.hpp"

namespace ScaleMail
{
//	============================================================================
AiNode::AiNode(Entity& entity, AiTree& parentTree)
: mParentTree(parentTree), mEntity(entity) {
}

//	============================================================================
Entity AiNode::getEntity() {
	return mEntity;
}

//	============================================================================
AiTree& AiNode::getParentTree() {
	return mParentTree;
}

//	============================================================================
AiWhiteboard& AiNode::getWhiteboard() {
	return mParentTree.getWhiteboard();
}
}