#pragma once

#include "entity.hpp"
#include "input_device.hpp"
#include <string>

namespace ScaleMail
{
class Player
{
public:
	Player(std::string name) : inputDevice(nullptr), name(name) {
	}

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	Player(Player&& rhs) :
		entity(std::move(rhs.entity)),
		inputDevice(std::move(rhs.inputDevice)),
		name(std::move(rhs.name)) {
	}

	Player& operator=(Player&& rhs) {
		entity = std::move(rhs.entity);
		inputDevice = std::move(rhs.inputDevice);
		name = std::move(rhs.name);
		return *this;
	}

	Entity entity;
	InputDevice* inputDevice;
	std::string name;
};
}