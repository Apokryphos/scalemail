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

    Entity entity;
    InputState inputState;
    std::string name;
};
}