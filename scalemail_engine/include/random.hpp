#pragma once

#include <random>
#include <vector>

namespace ScaleMail
{
class Random
{
	std::mt19937 mEngine;

public:
	Random() {
		std::random_device rd;
		std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
		mEngine = std::mt19937(seed);

	}

	template <typename T>
	T getRandomElement(const std::vector<T>& v) {
		if (v.size() == 0) {
			return nullptr;
		}

		std::uniform_int_distribution<int> dist(0, v.size() - 1);
		int index = dist(mEngine);
		return v[index];
	}

	float nextFloat(float min, float max) {
		return static_cast<float>(std::uniform_real_distribution<double>(min, max)(mEngine));
	}
};
}