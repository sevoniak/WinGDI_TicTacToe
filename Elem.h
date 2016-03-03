#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <iostream>
#include <sstream>
using namespace std;
#include <windows.h>

class Elem
{
	bool state;
	int symbol;

public:
	Elem(void);
	~Elem(void);

	void SetState();
	bool GetState();

	void SetSymbol(int);
	int GetSymbol();
};
