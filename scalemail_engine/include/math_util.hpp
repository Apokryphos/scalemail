#pragma once

#include <algorithm>
#include <cmath>
#include <glm/gtx/norm.hpp>

namespace ScaleMail
{
static const float PI = 3.141592653589793f;
static const float TWO_PI = 6.28318530718f;

//  ============================================================================
static inline int clamp( const int value, const int min, const int max ) {
	return std::max<int>( min, std::min<int>( value, max ) );
}

//  ============================================================================
static inline float clamp(
	const float value, const float min, const float max ) {
	return std::max<float>( min, std::min<float>( value, max ) );
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
static inline float toRadians(float degrees) {
	return degrees * PI / 180.0f;
}
}