#pragma once
#include <string>
#include <sgg/graphics.h>
using namespace std;

class Card
{
	float m_pos[2];
	int value = 0;
	std::string symbol = "";
	std::string name = "1";
	
public:
	Card(int v, string sv, string n)
	{
		value = v;
		symbol = sv;
		name = n;
	}
	
	string getName();
	
	int getValue();

	string getSymbol();
};