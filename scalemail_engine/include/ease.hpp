#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <functional>

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
inline float easeInOutSine(float t, float b, float c, float d) {
	return -c / 2 * (cos(M_PI * t / d) - 1) + b;
};

//  ============================================================================
inline float easeOutCubic(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * (t * t * t + 1) + b;
};

//  ============================================================================
inline glm::vec2 easeVec2(
	float t,
	glm::vec2 b,
	glm::vec2 c,
	float d,
	std::function<float(float, float, float, float)> easeFunc) {
	glm::vec2 out;
	out.x = easeFunc(t, b.x, c.x - b.x, d);
	out.y = easeFunc(t, b.y, c.y - b.y, d);
	return out;
}

//  ============================================================================
inline glm::vec3 easeVec3(
	float t,
	glm::vec3 b,
	glm::vec3 c,
	float d,
	std::function<float(float, float, float, float)> easeFunc) {
	glm::vec3 out;
	out.r = easeFunc(t, b.r, c.r - b.r, d);
	out.g = easeFunc(t, b.g, c.g - b.g, d);
	out.b = easeFunc(t, b.b, c.b - b.b, d);
	return out;
}

//  ============================================================================
inline glm::vec4 easeVec4(
	float t,
	glm::vec4 b,
	glm::vec4 c,
	float d,
	std::function<float(float, float, float, float)> easeFunc) {
	glm::vec4 out;
	out.r = easeFunc(t, b.r, c.r - b.r, d);
	out.g = easeFunc(t, b.g, c.g - b.g, d);
	out.b = easeFunc(t, b.b, c.b - b.b, d);
	out.a = easeFunc(t, b.a, c.a - b.a, d);
	return out;
}
}