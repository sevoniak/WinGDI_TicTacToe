#include "Elem.h"

Elem::Elem(void)
{
	state = false;
	symbol = -1;
}

Elem::~Elem(void)
{
}

void Elem::SetState() { state = !state; }
bool Elem::GetState() { return state; }
void Elem::SetSymbol(int symbolIN) { symbol = symbolIN; }
int Elem::GetSymbol() { return symbol; }