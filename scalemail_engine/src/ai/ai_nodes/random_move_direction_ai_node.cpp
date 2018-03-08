#include "ai/ai_nodes/random_move_direction_ai_node.hpp"
#include "math_util.hpp"
#include "world.hpp"
#include <glm/vec2.hpp>

namespace ScaleMail
{
//	============================================================================
RandomMoveDirectionAiNode::RandomMoveDirectionAiNode(
	Entity& entity, AiTree* parentTree)
: AiNode(entity, parentTree) {
}

//	============================================================================
AiNodeStatus RandomMoveDirectionAiNode::execute(World& world) {
	Entity entity = this->getEntity();

	Random& random = world.getRandom();

	//	Pick random direction
	glm::vec2 direction =
		rotateVec2(glm::vec2(1.0f, 0.0f), random.nextFloat(0.0f, TWO_PI));

	AiSystem& aiSystem = world.getAiSystem();
	AiComponent aiCmpnt = aiSystem.getComponent(entity);
	aiSystem.setMoveDirection(aiCmpnt, direction);

	return AiNodeStatus::SUCCESS;
}
}