#pragma once

namespace ScaleMail
{
struct SpriteFrame {
	float duration;
	int tilesetIds[4];
};

struct SpriteAnimation {
	int frameIndex;
	float ticks;
	SpriteFrame frames[2];
};
}