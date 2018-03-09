#pragma once

#include <algorithm>
#include <cmath>
#include <glm/gtx/norm.hpp>

namespace ScaleMail
{
static const float PI = 3.141592653589793f;
static const float PI_OVER_TWO = 1.57079632679f;
static const float TWO_PI = 6.28318530718f;

//  ============================================================================
template <typename T>
static inline T clamp(const T value, const T min, const T max) {
	return std::max<T>(min, std::min<T>(value, max));
}

//  ============================================================================
static inline bool isPowerOfTwo( const unsigned int value ) {
	return value && !( value & ( value - 1 ) );
}

//  ============================================================================
template <typename Vector>
inline Vector limit(const Vector &vector, float max) {
	float length = glm::length2(vector);
	if (length > max * max)	{
		return glm::normalize(vector) * max;
	} else {
		return vector;
	}
}

//  ============================================================================
static inline unsigned int nextPowerOfTwo( unsigned int value ) {
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;

	return value;
}

//	============================================================================
static inline glm::vec2 normalizeVec2(const glm::vec2& v) {
	if (glm::length2(v) != 0.0f) {
		return glm::normalize(v);
	} else {
		return v;
	}
}

//	============================================================================
static inline glm::vec2 rotateVec2(const glm::vec2& vector, float radians) {
	return glm::vec2(
		vector.x * std::cos(radians) - vector.y * std::sin(radians),
		vector.x * std::sin(radians) + vector.y * std::cos(radians));
}

//  ============================================================================
static inline int toQuadrant(const glm::vec2& direction)
{
	float angle = atan2(direction.y, direction.x);
	return (int)round(4.0f * angle / (2.0f * PI) + 4.0f) % 4;
}

//  ============================================================================
static inline float toRadians(float degrees) {
	return degrees * PI / 180.0f;
}

//  ============================================================================
//	Wraps a value to the range of one to zero
static inline float wrap(const float value) {
	if (value >= 0.0f && value <= 1.0f) {
		return value;
	}

	return std::abs(value - std::floor(value));
}
}