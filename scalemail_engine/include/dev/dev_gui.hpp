#pragma once

namespace ScaleMail
{
struct Game;

class DevGui
{
	bool mVisible;

public:
	DevGui();
	void draw(Game& game);
	bool getKeyboardInputConsumed() const;
	bool getMouseInputConsumed() const;
	bool getVisible() const;
	void setVisible(const bool visible);
};
}