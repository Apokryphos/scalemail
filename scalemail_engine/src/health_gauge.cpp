#include "health_gauge.hpp"
#include <algorithm>

namespace ScaleMail
{
//	============================================================================
HealthGauge::HealthGauge(float max) : mMax(max), mValue(max) {

}

//	============================================================================
void HealthGauge::add(float amount) {
	this->setValue(mValue + amount);
}

//	============================================================================
void HealthGauge::fill() {
	mValue = mMax;
}

//	============================================================================
float HealthGauge::getMax() const {
	return mMax;
}

//	============================================================================
float HealthGauge::getValue() const {
	return mValue;
}

//	============================================================================
bool HealthGauge::isEmpty() const {
	return mValue <= 0.0f;
}

//	============================================================================
void HealthGauge::setMax(float max) {
	mMax = std::max(max, 0.0f);
	mValue = std::min(mValue, mMax);
}

//	============================================================================
void HealthGauge::setValue(float value) {
	mValue = std::min(value, mMax);
}

//	============================================================================
void HealthGauge::subtract(float amount) {
	this->setValue(mValue - amount);
}
}