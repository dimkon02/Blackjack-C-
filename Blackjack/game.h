#pragma once
#include <list>
#include "card.h"
#include "animation.h"
#include <string>
#include <queue>

class Game
{
public:
	enum Game_State { INIT, WELCOMESCREEN, LOADING, PLACINGBET, DEALINGFIRSTCARDS, PLAYERSTURN, DEALERSTURN };
	float scpx = 23.0f;
	float scpy = 5.0f;
	int c = 0;
	int fpscounter = 0;
	int totalframes = 0;
	int avgfps = 0;
	int cardvalue = 2;
	int betamount = 0;
	int balance = 50000;
	std::string symb = "J";
	int c2 = 0;
	int betamount2 = 0;
	bool cardanimation = false;
	int cp = 0;
	int cd = 0;
	float carddistance = 0.0f;
	int deckcounter = 0;
	bool showlowbalancewarning = false;
	bool doubleaction = false;
	bool info = false;
	int playerscore = 0;
	int playerscore2 = 0;
	int buttons = 0; // 0 = HIT/STAND, 1 = DOUBLE, 2 = DOUBLE & SPLIT, 3 = SPLIT 
	int dealerscore = 0;
	int playeraction = 0;
	bool splitcards = false;
	int secondhandcounter = 0;
	int activehand = 1;
	int winner = 0; //1 = player, 2 = dealer , 3 = push
protected:
	static Game* m_instance;
	Card* closedcard = new Card(1, std::to_string(1), "cardback");
	Card* deck[52];
	Card* cards[312];
	Card* dealercards[20];
	Card* playercards[20];
	Card* playercards2ndhand[20];
	queue<animation*> animations;
	Game_State  game_state = INIT;

	Game() {}
public:
	void init();
	void draw();
	void update();
	static Game* getInstance();
	~Game();
};