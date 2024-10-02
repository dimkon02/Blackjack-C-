#include "sgg/graphics.h"
#include "defines.h"
#include <string>
#include "game.h"

void draw()
{
	Game * mygame = Game::getInstance();
	mygame->draw();
}

void  update(float ms)
{
	Game* mygame = Game::getInstance();
	mygame->update();
}


int main(int argc, char** argv)
{
	graphics::createWindow(1920, 1080, "Blackjack");

	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setFullScreen(true);

	Game* game = Game::getInstance();

	graphics::startMessageLoop();
	graphics::destroyWindow();
	delete game;
	return 0;
}