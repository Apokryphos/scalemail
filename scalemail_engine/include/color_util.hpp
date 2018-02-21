#pragma once

#include <glm/vec4.hpp>
#include <algorithm>
#include <cmath>

namespace ScaleMail
{
inline static float hueToRGB(float v1, float v2, float vH) {
	if (vH < 0.0f) {
		vH += 1.0f;
	} else if (vH > 1.0f) {
		vH -= 1.0f;
	}

	if ((6.0f * vH) < 1.0f) {
		return (v1 + (v2 - v1) * 6.0f * vH);
	}

	if ((2.0f * vH) < 1.0f) {
		return (v2);
	}

	if ((3.0f * vH) < 2.0f) {
		return (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f);
	}

	return (v1);
}

inline static glm::vec4 hslToRgb(const glm::vec4& hsl) {
	float hue = hsl[0];
	float sat = hsl[1];
	float lum = hsl[2];

	if (sat == 0.0f) {
		return glm::vec4(lum, lum, lum, hsl.a);
	} else {
		float v2;

		if (lum < 0.5f) {
			v2 = lum * (1 + sat);
		} else {
			v2 = (lum + sat) - (sat * lum);
		}

		float v1 = 2 * lum - v2;

		return glm::vec4(
			hueToRGB(v1, v2, hue + (1.0f / 3.0f)),
			hueToRGB(v1, v2, hue),
			hueToRGB(v1, v2, hue - (1.0f / 3.0f)),
			hsl.a
		);
	}
}

inline static glm::vec4 rgbToHsl(const glm::vec4& rgb) {
	float r = rgb.r;
	float g = rgb.g;
	float b = rgb.b;

	float vMin = std::min(std::min(r, g), b);
	float vMax = std::max(std::max(r, g), b);
	float delta = vMax - vMin;

	float hue, sat, lum;

	lum = (vMax + vMin)/ 2.0f;

	if (delta == 0)	{
		//	No chroma
		hue = 0;
		sat = 0;
	} else {
		if (lum < 0.5) sat = delta / (vMax + vMin);
		else           sat = delta / (2 - vMax - vMin);

		float del_R = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
		float del_G = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
		float del_B = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

		if (r == vMax) {
			hue = del_B - del_G;
		} else if (g == vMax) {
			hue = (1.0f / 3.0f) + del_R - del_B;
		} else if (b == vMax) {
			hue = (2.0f / 3.0f) + del_G - del_R;
		}

		if (hue < 0) hue += 1;
		if (hue > 1) hue -= 1;
	}

	return glm::vec4(hue, sat, lum, rgb.a);
}
}