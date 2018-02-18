#pragma once

#include "game_window.hpp"

namespace ScaleMail
{
class Camera;
class Gui;
class World;

struct Game
{
	bool cameraFollow;
	bool devMode;
	bool drawAi;
	bool drawCollision;
	bool paused;
	bool quit;
	double speed;
	Camera* camera;
	GameWindow gameWindow;
	Gui* gui;
	World* world;
};
}