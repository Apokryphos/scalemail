#pragma once

#include <stdexcept>

namespace ScaleMail
{
enum class VertexDefinition
{
	//	Fade
	POSITION2,

	//	Color quads (ambient light)
	//	Lines (debugging visuals)
	POSITION2_COLOR4,

	//	Fonts
	POSITION2_COLOR4_TEXTURE2,

	POSITION2_TEXTURE2,

	//	Tiles
	POSITION3_TEXTURE2,

	//	Sprite batch
	POSITION3_COLOR4_TEXTURE2,
};

//	============================================================================
inline int getVertexDefinitionAttributeCount(const VertexDefinition def) {
	switch (def) {
		case VertexDefinition::POSITION2:
			return 1;

		case VertexDefinition::POSITION2_COLOR4:
		case VertexDefinition::POSITION2_TEXTURE2:
		case VertexDefinition::POSITION3_TEXTURE2:
			return 2;

		case VertexDefinition::POSITION2_COLOR4_TEXTURE2:
		case VertexDefinition::POSITION3_COLOR4_TEXTURE2:
			return 3;

		default:
			throw std::runtime_error("Case not implemented.");
	}
}

//	============================================================================
inline int getVertexDefinitionElementCount(const VertexDefinition def) {
	switch (def) {
		case VertexDefinition::POSITION2:
			return 2;

		case VertexDefinition::POSITION2_TEXTURE2:
			return 4;

		case VertexDefinition::POSITION3_TEXTURE2:
			return 5;

		case VertexDefinition::POSITION2_COLOR4:
			return 6;

		case VertexDefinition::POSITION2_COLOR4_TEXTURE2:
			return 8;

		case VertexDefinition::POSITION3_COLOR4_TEXTURE2:
			return 9;

		default:
			throw std::runtime_error("Case not implemented.");
	}
}
}