#pragma once

namespace ScaleMail
{
//  ============================================================================
inline float linear(float t, float b, float c, float d) {
	return c * t / d + b;
};

//  ============================================================================
inline float easeInCubic(float t, float b, float c, float d) {
	t /= d;
	return c * t * t * t + b;
};

//  ============================================================================
inline float easeInOutCubic(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) {
		return c / 2 * t * t * t + b;
	}
	t -= 2;
	return c / 2 * (t * t * t + 2) + b;
};


//  ============================================================================
inline float easeOutCubic(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * (t * t * t + 1) + b;
};
}