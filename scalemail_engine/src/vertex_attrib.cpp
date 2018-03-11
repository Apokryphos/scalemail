#include "gl_headers.hpp"
#include "vertex_attrib.hpp"
#include "vertex_definition.hpp"
#include <stdexcept>

namespace ScaleMail
{
//	============================================================================
void disableVertexAttributes(const VertexDefinition def) {
	const int attributeCount = getVertexDefinitionAttributeCount(def);
	for (int a = 0; a < attributeCount; ++a) {
		glDisableVertexAttribArray(a);
	}
}

//	============================================================================
void enableVertexAttributes(const VertexDefinition def) {
	const int attributeCount = getVertexDefinitionAttributeCount(def);
	for (int a = 0; a < attributeCount; ++a) {
		glEnableVertexAttribArray(a);
	}
}

//	============================================================================
void setVertexAttributePointers(const VertexDefinition def) {
	switch (def) {
		case VertexDefinition::POSITION2:
			floatVertexAttribPointer(0, 2, 2, 0);
			break;

		case VertexDefinition::POSITION2_COLOR4:
			floatVertexAttribPointer(0, 2, 6, 0);
			floatVertexAttribPointer(1, 4, 6, 2);
			break;

		case VertexDefinition::POSITION2_TEXTURE2:
			floatVertexAttribPointer(0, 2, 4, 0);
			floatVertexAttribPointer(1, 2, 4, 2);
			break;

		case VertexDefinition::POSITION3_COLOR4:
			floatVertexAttribPointer(0, 3, 7, 0);
			floatVertexAttribPointer(1, 4, 7, 3);
			break;

		case VertexDefinition::POSITION3_TEXTURE2:
			floatVertexAttribPointer(0, 3, 5, 0);
			floatVertexAttribPointer(1, 2, 5, 3);
			break;

		case VertexDefinition::POSITION2_COLOR4_TEXTURE2:
			floatVertexAttribPointer(0, 2, 8, 0);
			floatVertexAttribPointer(1, 4, 8, 2);
			floatVertexAttribPointer(2, 2, 8, 6);
			break;

		case VertexDefinition::POSITION3_COLOR4_TEXTURE2:
			floatVertexAttribPointer(0, 3, 9, 0);
			floatVertexAttribPointer(1, 4, 9, 3);
			floatVertexAttribPointer(2, 2, 9, 7);
			break;

		default:
			throw std::runtime_error("Case not implemented.");
	}
}
}