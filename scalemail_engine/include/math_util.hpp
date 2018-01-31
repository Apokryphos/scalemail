#pragma once

#include <algorithm>

namespace ScaleMail
{
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
}