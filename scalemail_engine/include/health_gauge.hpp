#pragma once

#include <utility>

namespace ScaleMail
{
class HealthGauge
{
	float mMax;
	float mValue;

public:
	HealthGauge(float max = 100);
	HealthGauge(const HealthGauge&) = delete;
	HealthGauge& operator=(const HealthGauge&) = delete;

	HealthGauge(HealthGauge&& rhs) :
		mMax(rhs.mMax),
		mValue(rhs.mValue) {
	}

	HealthGauge& operator=(HealthGauge&& rhs) {
		mMax = rhs.mMax;
		mValue = rhs.mValue;
		return *this;
	}

	void add(float amount);
	void fill();
	float getMax() const;
	float getValue() const;
	bool isEmpty() const;
	void setMax(float max);
	void setValue(float value);
	void subtract(float amount);
};
}