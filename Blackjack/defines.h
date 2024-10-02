#pragma once

#include <random>
#include <stdlib.h>
#define CHIP_SOUND graphics::playSound(ASSET_PATH + std::string("chipssound1.mp3"), 0.8f, false);
#define CARD_SOUND graphics::playSound(ASSET_PATH + std::string("carddealing.mp3"), 0.5f, false);
#define CANVAS_WIDTH 28.0f
#define CANVAS_HEIGHT 16.0f
#define CARDSIZE 2.0f
#define PLAYERCARDPY 10.7f
#define CARDDISTANCE 0.4f
#define ASSET_PATH ".\\assets\\"

inline float distance(float x1, float y1, float x2, float y2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrtf(dx*dx + dy*dy);
}