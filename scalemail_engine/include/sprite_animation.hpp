#pragma once

#include <vector>

namespace ScaleMail
{
struct SpriteFrame {
	float duration;
	int tilesetIds[4];
};

struct SpriteAnimation {
	int frameCount;
	int frameIndex;
	float ticks;
	std::vector<SpriteFrame> frames;
};
}