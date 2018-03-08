#include "ai/ai_node.hpp"
#include "ai/ai_tree.hpp"

namespace ScaleMail
{
//	============================================================================
AiTree::AiTree(Entity entity, AiNode* rootNode)
: mTotalElapsedSeconds(0), mEntity(entity), mRootNode(rootNode) {
}

//	============================================================================
void AiTree::execute(World& world, double totalElapsedSeconds) {
	//	AI nodes don't update every frame so use the total elapsed time
	mTotalElapsedSeconds = totalElapsedSeconds;

	if (mRootNode != nullptr) {
		mWhiteboard.removeDeadEntities(world);
		mRootNode->execute(world);
	}
}

//	============================================================================
double AiTree::getElapsedSeconds(double lastTick) const {
	return mTotalElapsedSeconds - lastTick;
}

//	============================================================================
double AiTree::getTotalElapsedSeconds() const {
	return mTotalElapsedSeconds;
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