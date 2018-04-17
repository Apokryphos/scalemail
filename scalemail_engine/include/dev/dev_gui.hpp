#pragma once

#include "entity_list_box.hpp"

namespace ScaleMail
{
struct Game;

class DevGui
{
	bool mVisible;
	EntityListBox mEntityListBox;

	void drawEntityDebugWindows(Game& game);

public:
	DevGui();
	void draw(Game& game);
	bool getKeyboardInputConsumed() const;
	bool getMouseInputConsumed() const;
	bool getVisible() const;
	void setVisible(const bool visible);
};
}