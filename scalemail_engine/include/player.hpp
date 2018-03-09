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

	Player(Player&& player) :
		entity(std::move(player.entity)),
		inputState(std::move(player.inputState)),
		name(std::move(player.name)) {
	}

	Player& operator=(Player&& player) {
		entity = std::move(player.entity);
		inputState = std::move(player.inputState);
		name = std::move(player.name);
		return *this;
	}

	Entity entity;
	InputState inputState;
	std::string name;
};
}