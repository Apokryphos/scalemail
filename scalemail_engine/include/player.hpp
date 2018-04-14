#pragma once

#include "entity.hpp"
#include "input_device.hpp"
#include <string>

namespace ScaleMail
{
class Player
{
public:
	Player(std::string name) : actorIndex(0), inputDevice(nullptr), name(name) {
	}

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	Player(Player&& rhs) :
		actorIndex(std::move(rhs.actorIndex)),
		entity(std::move(rhs.entity)),
		inputDevice(std::move(rhs.inputDevice)),
		name(std::move(rhs.name)) {
	}

	Player& operator=(Player&& rhs) {
		actorIndex = std::move(rhs.actorIndex);
		entity = std::move(rhs.entity);
		inputDevice = std::move(rhs.inputDevice);
		name = std::move(rhs.name);
		return *this;
	}

	int actorIndex;
	Entity entity;
	InputDevice* inputDevice;
	std::string name;
};
}