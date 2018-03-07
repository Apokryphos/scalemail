#include "ai/ai_node.hpp"
#include "ai/ai_tree.hpp"

namespace ScaleMail
{
//	============================================================================
AiTree::AiTree(Entity entity, AiNode* rootNode)
: mEntity(entity), mRootNode(rootNode) {
}

//	============================================================================
Entity AiTree::getEntity() {
	return mEntity;
}

//	============================================================================
AiNode* AiTree::getRootNode() {
	return mRootNode.get();
}

//	============================================================================
AiWhiteboard& AiTree::getWhiteboard() {
	return mWhiteboard;
}

//	============================================================================
void AiTree::setRootNode(std::shared_ptr<AiNode> node) {
	mRootNode = node;
}
}