#include "gl_headers.hpp"

namespace ScaleMail
{
//  ============================================================================
void blendAdditive() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}

//  ============================================================================
void blendAlpha() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//  ============================================================================
void blendAlphaAdditive() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

//  ============================================================================
void blendModulate() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
}

//  ============================================================================
void blendNone() {
	glDisable(GL_BLEND);
}

//  ============================================================================
void blendScreen() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
}
}