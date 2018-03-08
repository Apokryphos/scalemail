#include "ai/ai_nodes/entity_count_ai_node.hpp"
#include "ai/ai_whiteboard.hpp"
#include "entity_util.hpp"
#include "damage_system.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
EntityCountAiNode::EntityCountAiNode(Entity& entity, AiTree* parentTree,
									 std::string valueName)
: AiNode(entity, parentTree), mCompareType(CompareType::GREATER_THAN_OR_EQUAL),
mCountMin(1), mCountMax(1), mValueName(valueName) {
}

//	============================================================================
AiNodeStatus EntityCountAiNode::execute(World& world,
										[[maybe_unused]]float elapsedSeconds) {
	AiWhiteboard& wb = this->getWhiteboard();

	int count = wb.getEntityCount(mValueName);

	bool match = false;

	switch (mCompareType) {
		case CompareType::EQUAL:
			match = count == mCountMin;
			break;

		case CompareType::GREATER_THAN:
			match = count > mCountMin;
			break;

		case CompareType::GREATER_THAN_OR_EQUAL:
			match = count >= mCountMin;
			break;

		case CompareType::LESS_THAN:
			match = count < mCountMin;
			break;

		case CompareType::LESS_THAN_OR_EQUAL:
			match = count <= mCountMin;
			break;

		case CompareType::RANGE:
			match = count >= mCountMin && count <= mCountMax;
			break;

		default:
			throw std::runtime_error("Case not implemented.");
	}

	return match ? AiNodeStatus::SUCCESS : AiNodeStatus::FAILURE;
}

//	============================================================================
void EntityCountAiNode::setEqualTo(const int value) {
	mCompareType = CompareType::EQUAL;
	mCountMin = value;
	mCountMax = value;
}

//	============================================================================
void EntityCountAiNode::setGreaterThanOrEqualTo(const int value) {
	mCompareType = CompareType::GREATER_THAN_OR_EQUAL;
	mCountMin = value;
	mCountMax = value;
}

//	============================================================================
void EntityCountAiNode::setRange(const int min, const int max) {
	mCompareType = CompareType::RANGE;
	mCountMin = min;
	mCountMax = max;
}

//	============================================================================
void EntityCountAiNode::setValueName(const std::string& name) {
	mValueName = name;
}
}