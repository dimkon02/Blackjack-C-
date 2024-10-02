#include "animation.h"
#include <sgg/graphics.h>
void playerCardAnimation::DrawAnimation()
{
	if (!done)
	{
		if (counter == 0)
		{
			CARD_SOUND;
			incx = (9.0f - amountofcards*0.4f) / lastframe;
			incy = 5.0f / lastframe;
			if (splitcards && activehand == 1)
			{
				incx = (6.1f - amountofcards * 0.4f) / lastframe;
			}
			else if (splitcards && activehand == 2)
			{
				incx = (11.9f - amountofcards * 0.4f) / lastframe;
			}
		}
		graphics::drawRect(scpx, scpy, CARDSIZE, CARDSIZE, br);
		scpx -= incx;
		scpy += incy;
		counter++;
		if (counter == lastframe) { scpx = 23.0f; scpy = 5.0f; done = true; }
	}
}

void dealerCardAimation::DrawAnimation()
{
	if (!done)
	{
		if (counter == 0) 
		{ 
			incx = (9.0f - amountofcards * 0.4f) / lastframe;
			CARD_SOUND; 
		}
		graphics::drawRect(scpx, scpy, CARDSIZE, CARDSIZE, br);
		scpx -= incx;
		counter++;
		if (counter == lastframe) { scpx = 23.0f; done = true; }
	}
}
