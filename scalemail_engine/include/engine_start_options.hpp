#pragma once

#include <string>

struct EngineStartOptions
{
	bool openGl2;
	bool skipIntro;
	int screenHeight;
	int screenWidth;
	std::string mapName;
};