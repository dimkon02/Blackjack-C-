#include "card.h"
#include "sgg/graphics.h"
#include "defines.h"

string Card::getName()
{
	return name;
}

int Card::getValue()
{
	return value;
}

string Card::getSymbol()
{
	return symbol;
}
