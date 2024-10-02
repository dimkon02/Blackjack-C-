#include "game.h"
#include "defines.h"
#include <sgg/graphics.h>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include <array>
#include <queue>

using namespace std;

void Game::init()
{
	//Creating one deck of cards
	for (int i = 0; i < 52; i++)
	{
		if (i < 4)
		{
			Card* c = new Card(11, std::to_string(1), std::to_string(i + 1));
			deck[i] = c;
		}
		else if (i<40)
		{
			Card* c = new Card(cardvalue, std::to_string(cardvalue), std::to_string(i + 1));
			deck[i] = c;
			if ((i + 1) % 4 == 0)
			{
				cardvalue += 1;
			}
		}
		else
		{
			Card* c = new Card(10, symb, std::to_string(i + 1));
			deck[i] = c;
			if (i + 1 == 44 )
			{
				symb = "K";
			}
			else if ( i + 1 == 48 )
			{
				symb = "Q";
			}
		}
	}
	//Adding 6 decks of cards to cards[]
	int count = 0;
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 0; j < 52; j++)
		{
			cards[j + count] = deck[j];
		}
		count += 52;
	}

	//Shuffling the cards;
	srand(time(0));
	for (int i = 0; i < 312; i++)
	{
		int r = i + (rand() % (312 - i));
		swap(cards[i], cards[r]);
	}
	
	//Preloading assets
	graphics::preloadBitmaps(ASSET_PATH);
}

void Game::draw()
{
	//Calculating the average framerate to be used for animations
	totalframes += (int)(1 / (graphics::getDeltaTime() / 1000.0f));
	fpscounter++;
	if (fpscounter == 200)
	{
		avgfps = (int)(totalframes / 200);
		fpscounter = 0;
		totalframes = 0;
	}
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	graphics::setFont("FreeSans.ttf");
	//Draw according to the state of the game(update has been called first)
	
	if (game_state == INIT)
	{
		graphics::drawText(25.6f, 15.85f, 0.5f, "LOADING...", br);
		game_state = LOADING;
		return;
	}
	if (game_state == LOADING) {
		graphics::drawText(25.6f, 15.85f, 0.5f, "LOADING...", br);
		br.texture = ASSET_PATH + std::string("loadingimg.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 6.0f, 7.0f, br);
		return;
	}
	if (game_state == WELCOMESCREEN)
	{
		br.texture = ASSET_PATH + std::string("welcomescreen.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		return;
	}
	if (game_state == PLACINGBET)
	{
		br.texture = ASSET_PATH + std::string("bb2.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		//Drawing info button
		br.texture = ASSET_PATH + std::string("infobtn.png");
		graphics::drawRect(27.3f, 0.7f, 0.5f, 0.5f, br);
		//$5chip
		br.texture = ASSET_PATH + std::string("5$chip.png");
		graphics::drawRect(8.0f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//$25chip
		br.texture = ASSET_PATH + std::string("25$chip.png");
		graphics::drawRect(10.4f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//$50chip
		br.texture = ASSET_PATH + std::string("50$chip.png");
		graphics::drawRect(12.8f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//$100chip
		br.texture = ASSET_PATH + std::string("100$chip.png");
		graphics::drawRect(15.2f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//$500chip
		br.texture = ASSET_PATH + std::string("500$chip.png");
		graphics::drawRect(17.6f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//$2500chip
		br.texture = ASSET_PATH + std::string("2500$chip.png");
		graphics::drawRect(20.0f, CANVAS_HEIGHT / 2, 2.0f, 2.0f, br);
		//bet button
		br.texture = ASSET_PATH + std::string("betbtn.png");
		graphics::drawRect(CANVAS_WIDTH / 2, 10.0f, 1.5f, 1.0f, br);
		//show Balance and bet amount
		graphics::drawText(1.35f, 15.4f, 0.4f, "$"+std::to_string(balance), br);
		graphics::drawText(25.35f, 15.4f, 0.4f, "$" + std::to_string(betamount+betamount2), br);
		//show balance refill button
		br.texture = ASSET_PATH + std::string("refillbalance.png");
		graphics::drawRect(4.5f, 15.1f, 0.8f, 0.8f, br);
		//Show info if needed
		if (info)
		{
			br.texture = ASSET_PATH + std::string("info.png");
			graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 24.0f, 13.5f, br);
		}
		//Show low balance warning if needed
		if (showlowbalancewarning)
		{
			br.texture = ASSET_PATH + std::string("lowbalance.png");
			graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
			c += 1;
			if (c == 200)
			{
				c = 0;
				showlowbalancewarning = false;
			}
		}
		return;
	}
	if (game_state == DEALINGFIRSTCARDS)
	{
		br.texture = ASSET_PATH + std::string("bb2.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		//Drawing the card animations for the first 4 cards
		
		if (!animations.empty())
		{
			if (animations.size() == 3)
			{
				br.texture = ASSET_PATH + playercards[0]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2, 10.0f, CARDSIZE, CARDSIZE, br);
			}
			else if (animations.size() == 2)
			{
				br.texture = ASSET_PATH + playercards[0]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2, 10.0f, CARDSIZE, CARDSIZE, br);
				br.texture = ASSET_PATH + playercards[1]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2 + 0.4f, 10.0f, CARDSIZE, CARDSIZE, br);
			}
			else if (animations.size() == 1)
			{
				br.texture = ASSET_PATH + playercards[0]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2, 10.0f, CARDSIZE, CARDSIZE, br);
				br.texture = ASSET_PATH + playercards[1]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2 + 0.4f, 10.0f, CARDSIZE, CARDSIZE, br);
				br.texture = ASSET_PATH + dealercards[0]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2, 5.0f, CARDSIZE, CARDSIZE, br);
			}
			animations.front()->DrawAnimation();
			if (animations.front()->getStatus())
			{
				animations.pop();
			}
		}
		else
		{
			for (int i = 0; i <= 1; i++)
			{
				br.texture = ASSET_PATH + playercards[i]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2 + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
				carddistance += 0.4f;
			}
			br.texture = ASSET_PATH + dealercards[0]->getName() + ".png";
			graphics::drawRect(CANVAS_WIDTH / 2, 5.0f, CARDSIZE, CARDSIZE, br);
			br.texture = ASSET_PATH + std::string("cardback.png");
			graphics::drawRect(CANVAS_WIDTH / 2 + 0.4f, 5.0f, CARDSIZE, CARDSIZE, br);
		}
		// Drawing player's and dealers's cards total
		br.texture = ASSET_PATH + std::string("cardcount.png");
		graphics::drawRect(12.5f, 10.0f, 0.6f, 0.45f, br);
		graphics::drawRect(12.5f, 5.0f, 0.6f, 0.45f, br);
		graphics::drawText(12.34f, 10.14f, 0.4f, std::to_string(playerscore), br);
		graphics::drawText(12.34f, 5.14f, 0.4f, std::to_string(dealercards[0]->getValue()), br);

		carddistance = 0.0f;
		//Drawing balance and bet amount
		graphics::drawText(1.35f, 15.4f, 0.4f, "$" + std::to_string(balance), br);
		graphics::drawText(25.35f, 15.4f, 0.4f, "$" + std::to_string(betamount+betamount2), br);
	
		if (animations.empty()) 
		{ 
			game_state = PLAYERSTURN; 
		}
		//Drawing the chips on the table
		br.texture = ASSET_PATH + std::string("stackedchips.png");
		graphics::drawRect(CANVAS_WIDTH / 2, 11.75f, 1.4f, 1.39f, br);
		return;
	}
	if (game_state == PLAYERSTURN)
	{
		br.texture = ASSET_PATH + std::string("bb2.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		br.texture = ASSET_PATH + std::string("stackedchips.png");
		graphics::drawRect(CANVAS_WIDTH / 2, 11.75f, 1.4f, 1.39f, br);
		graphics::drawText(1.35f, 15.4f, 0.4f, "$" + std::to_string(balance), br);
		graphics::drawText(25.35f, 15.4f, 0.4f, "$" + std::to_string(betamount+betamount2), br);

		//Drawing cards and animations if the player clicked hit to get 1 more card
		carddistance = 0;

		if (splitcards)
		{
			for (int i = 0; i <= cp; i++)
			{
				if (!animations.empty() && i == cp && animations.front()->activehand == 1)
				{
					animations.front()->DrawAnimation();
					if (animations.front()->getStatus())
					{
						animations.pop();
					}
				}
				else
				{
					br.texture = ASSET_PATH + playercards[i]->getName() + ".png";
					graphics::drawRect(16.5f + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
					carddistance += 0.4f;
				}
			}
			carddistance = 0;
			for (int i = 0; i <= secondhandcounter; i++)
			{
				if (!animations.empty() && i == secondhandcounter && animations.front()->activehand==2)
				{
					animations.front()->DrawAnimation();
					if (animations.front()->getStatus())
					{
						animations.pop();
					}
				}
				else
				{
					br.texture = ASSET_PATH + playercards2ndhand[i]->getName() + ".png";
					graphics::drawRect(11.5 + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
					carddistance += 0.4f;
				}
			}
			carddistance = 0;
			// Drawing player's and dealers's cards total	
			br.texture = ASSET_PATH + std::string("cardcount.png");
			graphics::drawRect(12.5f, 5.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 5.14f, 0.4f, std::to_string(dealercards[0]->getValue()), br);
			graphics::drawRect(10.0f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(9.84f, 10.14f, 0.4f, std::to_string(playerscore2), br);
			graphics::drawRect(15.0f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(14.84f, 10.14f, 0.4f, std::to_string(playerscore), br);
			//Drawing active hand identifier
			br.texture = ASSET_PATH + std::string("activehand.png");
			if (activehand == 1)
			{
				graphics::drawRect(16.5f, 11.5f, 1.5f, 1.5f, br);
			}
			else
			{
				graphics::drawRect(11.5f, 11.5f, 1.5f, 1.5f, br);
			}
		}
		else
		{
			for (int i = 0; i <= cp; i++)
			{
				if (!animations.empty() && i == cp)
				{
					animations.front()->DrawAnimation();
					if (animations.front()->getStatus())
					{
						animations.pop();
					}
				}
				else
				{
					br.texture = ASSET_PATH + playercards[i]->getName() + ".png";
					graphics::drawRect(CANVAS_WIDTH / 2 + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
					carddistance += 0.4f;
				}
			}
			// Drawing player's and dealers's cards total
			br.texture = ASSET_PATH + std::string("cardcount.png");
			graphics::drawRect(12.5f, 5.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 5.14f, 0.4f, std::to_string(dealercards[0]->getValue()), br);
			graphics::drawRect(12.5f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 10.14f, 0.4f, std::to_string(playerscore), br);
		}
		carddistance = 0.0f;
		br.texture = ASSET_PATH + dealercards[0]->getName() + ".png";
		graphics::drawRect(CANVAS_WIDTH / 2, 5.0f, CARDSIZE, CARDSIZE, br);
		br.texture = ASSET_PATH + std::string("cardback.png");
		graphics::drawRect(CANVAS_WIDTH / 2 + 0.4f, 5.0f, CARDSIZE, CARDSIZE, br);
		//Drawing buttons
		if (((splitcards && playerscore2 < 21) || (!splitcards && playerscore < 21)) && animations.empty() && !doubleaction)
		{
			br.texture = ASSET_PATH + std::string("HitButton.png");
			graphics::drawRect(13.05f, 7.5f, 1.5f, 1.5f, br);
			br.texture = ASSET_PATH + std::string("StandButton.png");
			graphics::drawRect(14.95f, 7.5f, 1.5f, 1.5f, br);
			if (buttons == 1 || buttons == 2)
			{
				br.texture = ASSET_PATH + std::string("DoubleButton.png");
				graphics::drawRect(11.2, 7.5f, 1.5f, 1.0f, br);
			}
			if (buttons == 2 || buttons == 3)
			{
				br.texture = ASSET_PATH + std::string("SplitButton.png");
				graphics::drawRect(16.8f, 7.5f, 1.5f, 1.1f, br);
			}
		}
		if ((playerscore >= 21 || doubleaction) && animations.empty())
		{
			if (splitcards)
			{
				activehand = 2;
			}
			else
			{
				 game_state = DEALERSTURN; 
			}
		}
		if ((playerscore2 >= 21 || doubleaction) && animations.empty())
		{
			game_state = DEALERSTURN;
		}
		return;
	}
	if (game_state == DEALERSTURN)
	{
		br.texture = ASSET_PATH + std::string("bb2.png");
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		br.texture = ASSET_PATH + std::string("stackedchips.png");
		graphics::drawRect(CANVAS_WIDTH / 2, 11.75f, 1.4f, 1.39f, br);
		graphics::drawText(1.35f, 15.4f, 0.4f, "$" + std::to_string(balance), br);
		graphics::drawText(25.35f, 15.4f, 0.4f, "$" + std::to_string(betamount+betamount2), br);
		//Drawing cards
		carddistance = 0;

		if (splitcards)
		{
			for (int i = 0; i <= cp; i++)
			{
				br.texture = ASSET_PATH + playercards[i]->getName() + ".png";
				graphics::drawRect(16.5f + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
				carddistance += 0.4f;
			}
			carddistance = 0;
			for (int i = 0; i <= secondhandcounter; i++)
			{
				br.texture = ASSET_PATH + playercards2ndhand[i]->getName() + ".png";
				graphics::drawRect(11.5 + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
				carddistance += 0.4f;
			}
			carddistance = 0;
			br.texture = ASSET_PATH + std::string("cardcount.png");
			graphics::drawRect(12.5f, 5.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 5.14f, 0.4f, std::to_string(dealerscore), br);
			graphics::drawRect(10.0f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(9.84f, 10.14f, 0.4f, std::to_string(playerscore2), br);
			graphics::drawRect(15.0f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(14.84f, 10.14f, 0.4f, std::to_string(playerscore), br);
		}
		else
		{
			for (int i = 0; i <= cp; i++)
			{
				br.texture = ASSET_PATH + playercards[i]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2 + carddistance, 10.0f, CARDSIZE, CARDSIZE, br);
				carddistance += 0.4f;
			}
			// Drawing player's and dealers's cards total
			br.texture = ASSET_PATH + std::string("cardcount.png");
			graphics::drawRect(12.5f, 5.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 5.14f, 0.4f, std::to_string(dealerscore), br);
			graphics::drawRect(12.5f, 10.0f, 0.6f, 0.45f, br);
			graphics::drawText(12.34f, 10.14f, 0.4f, std::to_string(playerscore), br);
			
		}
		carddistance = 0.0f;
		//drawing dealer cards and animations if needed
		for (int i = 0; i <= cd; i++)
		{
			if (!animations.empty() && i == cd)
			{
				animations.front()->DrawAnimation();
				if (animations.front()->getStatus())
				{
					animations.pop();
				}
			}
			else
			{
				br.texture = ASSET_PATH + dealercards[i]->getName() + ".png";
				graphics::drawRect(CANVAS_WIDTH / 2 + carddistance, 5.0f, CARDSIZE, CARDSIZE, br);
				carddistance += 0.4f;
			}
		}
		//Drawing results
		if (winner != 0 && c < 150)
		{
			c++;
			return;
		}
		else if (winner != 0 && c >= 150)
		{
			br.texture = ASSET_PATH + std::string("refillbalance.png");
			graphics::drawRect(4.5f, 15.1f, 0.8f, 0.8f, br);
		}
		if (winner == 1)
		{
			br.texture = ASSET_PATH + std::string("winnotice.png");
			graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
			graphics::drawText(12.0f, 15.5f, 0.5f, "PRESS ESC TO EXIT", br);
		}
		else if (winner == 2)
		{
			br.texture = ASSET_PATH + std::string("dealerwins.png");
			graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
			graphics::drawText(12.0f, 15.5f, 0.5f, "PRESS ESC TO EXIT", br);
		}
		else if (winner == 3)
		{
			br.texture = ASSET_PATH + std::string("push.png");
			graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
			graphics::drawText(12.0f, 15.5f, 0.5f, "PRESS ESC TO EXIT", br);
		}
	}
}

Game* Game::m_instance = nullptr;

void Game::update()
{
	//Updating according to game_state
	if (game_state == INIT)
	{
		return;
	}
	if (game_state == LOADING) {
		init();
		graphics::playMusic( ASSET_PATH + std::string("Twilight Dreams.mp3"), 0.5f, true, 0.0f);
		game_state = WELCOMESCREEN;
		return;
	}
	if (game_state == WELCOMESCREEN)
	{
		if (graphics::getKeyState(graphics::SCANCODE_RETURN))
		{
			game_state = PLACINGBET;
		}
		return;
	}
	if (game_state == PLACINGBET)
	{
		graphics::MouseState ms;
		graphics::getMouseState(ms);
		float mx = graphics::windowToCanvasX(ms.cur_pos_x);
		float my = graphics::windowToCanvasY(ms.cur_pos_y);
		//Showing info if needed
		if (distance(27.3f, 0.7f, mx, my) < 0.25f) { info = true; }
		else { info = false; }
		//Taking input for the bet amount
		if (ms.button_left_pressed && !showlowbalancewarning)
		{
			if (distance(8.0f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 5) { betamount += 5; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(10.4f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 25) { betamount += 25; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(12.8f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 50) { betamount += 50; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(15.2f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 100) { betamount += 100; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(17.6f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 500) { betamount += 500; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(20.0f, 8.0f, mx, my) < 1)
			{
				if (balance - betamount >= 2500) { betamount += 2500; CHIP_SOUND }
				else { showlowbalancewarning = true; }
			}
			else if (distance(4.5f, 15.1f, mx, my) <= 0.4)
			{
				balance = 50000;
			}
			else if (distance(CANVAS_WIDTH / 2, 10.0f, mx, my) < 0.6f && betamount > 0)
			{
				//Giving out the first 4 cards
				balance -= betamount;
				game_state = DEALINGFIRSTCARDS;
				c = 0;
				playercards[cp] = cards[deckcounter];
				playerscore += cards[deckcounter]->getValue();
				cp++;
				deckcounter++;
				playercards[cp] = cards[deckcounter];
				playerscore += cards[deckcounter]->getValue();
				deckcounter++;
				dealercards[cd] = cards[deckcounter];
				dealerscore += cards[deckcounter]->getValue();
				cd++;
				deckcounter++;
				dealercards[cd] = cards[deckcounter];
				dealerscore += cards[deckcounter]->getValue();
				deckcounter++;
				CHIP_SOUND;

				//animation code
				playerCardAnimation* an = new playerCardAnimation(avgfps, playercards[0], activehand, splitcards, cp);
				animations.push(an);
				an = new playerCardAnimation(avgfps, playercards[1], activehand, splitcards, cp);
				animations.push(an);
				dealerCardAimation* an2 = new dealerCardAimation(avgfps, dealercards[0], activehand, splitcards, cd);
				animations.push(an2);
				an2 = new dealerCardAimation(avgfps, closedcard, activehand, splitcards, cd);
				animations.push(an2);
				//animation code
			}
		}

		
		return;
	}
	if (game_state == DEALINGFIRSTCARDS)
	{
		return;
	}
	if (game_state == PLAYERSTURN)
	{
		//Determining if the player is allowed to hit,stand,double or split the cards
		if ((playerscore >= 9 && playerscore <= 11) && balance >= betamount && !splitcards && cp==1)
		{
			buttons = 1;
			if (playercards[0]->getValue() == playercards[1]->getValue() && playercards[0]->getSymbol() == playercards[1]->getSymbol()) { buttons = 2; }

		}
		else if ((playercards[0]->getValue() == playercards[1]->getValue()) && playercards[0]->getSymbol() == playercards[1]->getSymbol() && cp == 1 && balance >= betamount && !splitcards) { buttons = 3; }
		else { buttons = 0; }

		graphics::MouseState ms;
		graphics::getMouseState(ms);
		float mx = graphics::windowToCanvasX(ms.cur_pos_x);
		float my = graphics::windowToCanvasY(ms.cur_pos_y);
		//Taking input for the players action (hit,stand,double,split)
		if (ms.button_left_pressed)
		{
			if (distance(13.05f, 7.5f, mx, my) < 0.8f)
			{
				playeraction = 2;
			}
			else if (distance(14.95f, 7.5f, mx, my) < 0.8f)
			{
				playeraction = 3; 
			}
			else if ((buttons == 1 || buttons == 2) && distance(11.2, 7.5f, mx, my) < 0.7f)
			{
				playeraction = 1;
			}
			else if ((buttons == 2 || buttons == 3) && distance(16.8f, 7.5f, mx, my) < 0.7f)
			{
				playeraction = 4;
			}
			if (playeraction != 0)
			{
				if (playeraction == 3)
				{
					game_state = DEALERSTURN;
					if (splitcards && (activehand == 1)) 
					{ 
						activehand = 2;
						game_state = PLAYERSTURN;
					}
					playeraction = 0;
				}
				else if (playeraction == 2)
				{
					if (activehand == 1)
					{
						cp++;
						playercards[cp] = cards[deckcounter];
						playerscore += cards[deckcounter]->getValue();
						deckcounter++;
						playerCardAnimation* an = new playerCardAnimation(avgfps, playercards[cp], activehand, splitcards, cp);
						animations.push(an);
					}
					else if (activehand == 2)
					{
						secondhandcounter++;
						playercards2ndhand[secondhandcounter] = cards[deckcounter];
						playerscore2 += cards[deckcounter]->getValue();
						deckcounter++;
						playerCardAnimation* an = new playerCardAnimation(avgfps, playercards2ndhand[secondhandcounter], activehand, splitcards, secondhandcounter);
						animations.push(an);
					}
					playeraction = 0;
				}
				else if (playeraction == 1)
				{
					balance -= betamount;
					betamount += betamount;
					cp++;
					playercards[cp] = cards[deckcounter];
					playerscore += cards[deckcounter]->getValue();
					deckcounter++;
					playerCardAnimation* an = new playerCardAnimation(avgfps, playercards[cp], activehand, splitcards, cp);
					animations.push(an);
					doubleaction = true;
					playeraction = 0;
				}
				else if (playeraction == 4)
				{
					balance -= betamount;
					betamount2 = betamount;
					splitcards = true;
					playercards2ndhand[0] = playercards[1];
					playerscore2 = playercards[1]->getValue();
					playerscore -= playercards[1]->getValue();
					cp -= 1;
					playeraction = 0;
				}
			}

		}
		
	}
	if (game_state == DEALERSTURN)
	{
		//Determining the winner
		activehand = 1;
		if (winner == 0)
		{
			if (playerscore > 21 || (playerscore > 21 && playerscore2 > 21))
			{
				winner = 2;
			}
			else if (dealerscore < 17 && animations.empty())
			{
				cd++;
				dealercards[cd] = cards[deckcounter];
				dealerscore += cards[deckcounter]->getValue();
				deckcounter++;
				dealerCardAimation* an2 = new dealerCardAimation(avgfps, dealercards[cd], activehand, splitcards, cd);
				animations.push(an2);
			}
			else if (dealerscore >= 17)
			{
				if (dealerscore > 21)
				{
					balance += betamount * 1.5f;
					winner = 1;
					if (splitcards && playerscore2 <= 21)
					{
						balance += betamount * 1.5f;
						winner = 1;
					}
				}
				else if (playerscore > dealerscore)
				{
					balance += betamount * 1.5f;
					winner = 1;
					if (splitcards && playerscore2 > dealerscore)
					{
						balance += betamount * 1.5f;
						winner = 1;
					}
				}
				else if (playerscore == dealerscore)
				{
					balance += betamount;
					winner = 3;
					if (splitcards && playerscore2 == dealerscore)
					{
						balance += betamount;
						winner = 1;
					}
				}
				else { winner = 2; }

			}
		}
		//Preparing for the next game
		graphics::MouseState ms;
		graphics::getMouseState(ms);
		float mx = graphics::windowToCanvasX(ms.cur_pos_x);
		float my = graphics::windowToCanvasY(ms.cur_pos_y);
		if (ms.button_left_pressed && distance(CANVAS_WIDTH / 2, 9.5f, mx, my)<=1.0f)
		{
			game_state = PLACINGBET;
			cp = 0;
			cd = 0;
			c = 0;
			betamount = 0;
			betamount2 = 0;
			playerscore = 0;
			playerscore2 = 0;
			cardanimation = false;
			dealerscore = 0;
			animations = queue<animation*>();
			buttons = 0;
			splitcards = false;
			doubleaction = false;
			secondhandcounter = 0;
			winner = 0;
			if (deckcounter >= 200) //Shuffle cards if there are only 112 cards or less left in the shoe
			{
				srand(time(0));
				for (int i = 0; i < 312; i++)
				{
					// Random for remaining positions.
					int r = i + (rand() % (312 - i));

					swap(cards[i], cards[r]);
				}
				deckcounter = 0;
			}
		}
		else if (ms.button_left_pressed && distance(4.5f, 15.1f, mx, my) <= 0.4)
		{
			balance = 50000;
		}

	}

}

Game* Game::getInstance()
{
	if (!m_instance)
		m_instance = new Game();
	return m_instance;
}

Game::~Game()
{
	for (auto j : deck)
	{
		delete j;
	}
}