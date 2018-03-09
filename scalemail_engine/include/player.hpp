#pragma once

#include "entity.hpp"
#include "input_state.hpp"
#include <string>

namespace ScaleMail
{
class Player
{
public:
	Player(std::string name) : name(name) {
		inputState = {};
	}

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	Player(Player&& rhs) :
		entity(std::move(rhs.entity)),
		inputState(std::move(rhs.inputState)),
		name(std::move(rhs.name)) {
	}

	Player& operator=(Player&& rhs) {
		entity = std::move(rhs.entity);
		inputState = std::move(rhs.inputState);
		name = std::move(rhs.name);
		return *this;
	}

	Entity entity;
	InputState inputState;
	std::string name;
};
}