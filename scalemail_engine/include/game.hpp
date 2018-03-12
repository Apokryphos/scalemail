#pragma once

#include "game_window.hpp"
#include "render_caps.hpp"
#include "render_options.hpp"
#include "shader_version.hpp"

namespace ScaleMail
{
class Camera;
class Gui;
class World;

struct DevOptions
{
	bool camera3d;
	bool drawAi;
	bool drawCollision;
	bool drawTriggers;
	bool enabled;
	bool hideGui;
	bool step;
	int stepCount;
};

struct Game
{
	bool cameraFollow;
	bool drawAi;
	bool drawCollision;
	bool paused;
	bool quit;
	double speed;

	DevOptions devOptions;
	RenderCaps renderCaps;
	RenderOptions renderOptions;

	Camera* camera;
	GameWindow gameWindow;
	Gui* gui;
	World* world;
};
}