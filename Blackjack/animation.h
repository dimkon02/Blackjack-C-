#pragma once
#include "defines.h"
#include "card.h"
#include <sgg/graphics.h>

class animation
{

public:
	int lastframe = 0;
	int counter = 0;
	int activehand = 1;
	int amountofcards = 0;
	float scpx = 23.0f;
	float scpy = 5.0f;
	float incx = 0.0f;
	bool done = false;
	bool splitcards = false;
	graphics::Brush br;
	Card* card;
	animation() { return; }
	bool getStatus() { return done; }
	virtual void DrawAnimation() {}
};

class playerCardAnimation :public animation
{
public:
	float incy = 0.0f;
	playerCardAnimation(int avgfps, Card* c, int ah, bool sh, int ac)
	{
		lastframe = (int)(0.5f * avgfps);
		card = c;
		activehand = ah;
		splitcards = sh;
		amountofcards = ac;
		br.outline_opacity = 0.0f;
		br.texture = ASSET_PATH + card->getName() + ".png";
	}
	virtual void DrawAnimation();
};

class dealerCardAimation :public animation
{
public:
	dealerCardAimation(int avgfps, Card* c, int ah, bool sh, int ac)
	{
		lastframe = (int)(0.5f * avgfps);
		card = c;
		activehand = ah;
		splitcards = sh;
		amountofcards = ac;
		br.outline_opacity = 0.0f;
		br.texture = ASSET_PATH + card->getName() + ".png";
	}
	virtual void DrawAnimation();
};