#pragma once

namespace ScaleMail
{
class HealthGauge
{
	float mMax;
	float mValue;

public:
	HealthGauge(float max = 100);
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