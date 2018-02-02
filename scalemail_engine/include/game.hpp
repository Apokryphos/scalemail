#pragma once

#include "camera.hpp"
#include "game_window.hpp"
#include "world.hpp"

namespace ScaleMail
{
struct Game
{
	bool paused;
	bool quit;
	double speed;
	Camera* camera;
	GameWindow gameWindow;
	World* world;
};
}