#pragma once

#include "input_state.hpp"

namespace ScaleMail
{
class InputDevice
{
protected:
	InputState mInputState;

public:
	InputState& getInputState();
};
}