#pragma once

#include <optional>
#include <random>
#include <unordered_set>
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
	std::optional<T> getRandomOptionalElement(const std::vector<T>& v) {
		if (v.size() == 0) {
			return {};
		}

		std::uniform_int_distribution<int> dist(0, v.size() - 1);
		int index = dist(mEngine);
		return v[index];
	}

	template <typename T>
	T getRandomElement(const std::unordered_set<T>& s) {
		assert(s.size() > 0);

		std::uniform_int_distribution<int> dist(0, s.size() - 1);
		int index = dist(mEngine);

		return *std::next(std::begin(s), index);
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

	int nextInt(int min, int max) {
		return static_cast<int>(std::uniform_real_distribution<double>(min, max)(mEngine));
	}
};
}