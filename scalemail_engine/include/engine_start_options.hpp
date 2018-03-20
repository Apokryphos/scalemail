#pragma once

#include <string>

struct EngineStartOptions
{
	bool fullscreen;
	bool openGl2;
	bool skipIntro;
	int screenHeight;
	int screenWidth;
	std::string mapName;
};