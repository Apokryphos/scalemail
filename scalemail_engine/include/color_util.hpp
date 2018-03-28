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
	float light = hsl[2];

	if (sat == 0.0f) {
		return glm::vec4(light, light, light, hsl.a);
	} else {
		float v2;

		if (light < 0.5f) {
			v2 = light * (1 + sat);
		} else {
			v2 = (light + sat) - (sat * light);
		}

		float v1 = 2 * light - v2;

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
	float d = vMax - vMin;

	float hue = 0;
	float sat = 0;
	float light = 0;

	light = (vMax + vMin)/ 2.0f;

	if (d == 0)	{
		//	No chroma
		hue = 0;
		sat = 0;
	} else {
		if (light < 0.5) {
			sat = d / (vMax + vMin);
		} else {
			sat = d / (2 - vMax - vMin);
		}

		float dR = (((vMax - r) / 6.0f) + (d / 2.0f)) / d;
		float dG = (((vMax - g) / 6.0f) + (d / 2.0f)) / d;
		float dB = (((vMax - b) / 6.0f) + (d / 2.0f)) / d;

		if (r == vMax) {
			hue = dB - dG;
		} else if (g == vMax) {
			hue = (1.0f / 3.0f) + dR - dB;
		} else if (b == vMax) {
			hue = (2.0f / 3.0f) + dG - dR;
		}

		if (hue < 0.0f) {
			hue += 1.0f;
		} else if (hue > 1.0f) {
			hue -= 1.0f;
		}
	}

	return glm::vec4(hue, sat, light, rgb.a);
}
}