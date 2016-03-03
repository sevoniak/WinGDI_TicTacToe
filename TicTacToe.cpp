#include "Elem.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool CheckWin(Elem*, int&);
bool Compare(Elem*, Elem*, Elem*);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"TicTac";
	WNDCLASS wc = {};
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClass(&wc);
	HWND hwnd = CreateWindow(CLASS_NAME, L"Aire TicTacToe", WS_OVERLAPPEDWINDOW &~ WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT,
							 350, 400, NULL, NULL, hInstance, NULL);
	if (hwnd == 0)
		return 0;

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	PAINTSTRUCT ps;
	HDC hdc;
	static Elem* pElem;
	static Elem* pLoc;
	static int xMouse, yMouse, xCoord, yCoord, index, turn, numTurns;
	static bool gameOver, resetBoard, validPlay;
	static int winType;
	
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_CREATE:
		{
			pElem = new Elem[9];
			gameOver = false;
			resetBoard = false;
			validPlay = true;
			winType = 0;
			numTurns = 0;
			return 0;
		}

	case WM_SIZE:
		{
			cxClient = LOWORD(lParam) - 20;
			cyClient = HIWORD(lParam) - 50;
			return 0;
		}

	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			for (int i = 1; i < 3; i++)
			{
				MoveToEx(hdc, i * cxClient / 3 + 10, 10, NULL);
				LineTo(hdc, i * cxClient / 3 + 10, cyClient + 10);
				MoveToEx(hdc, 10, i * cyClient / 3 + 10, NULL);
				LineTo(hdc, cxClient +10 , i * cyClient / 3 + 10);
			}
			pLoc = pElem;
			for (int i = 0; i < 9; i++)
			{
				if (pLoc->GetState())
				{
					yCoord = i / 3;
					xCoord = i % 3;
					HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
					SelectObject(hdc, hpen);
					if (pLoc->GetSymbol() == 1)
					{
						MoveToEx(hdc, 20 + xCoord * cxClient / 3, 20 + yCoord * cyClient / 3, NULL);
						LineTo(hdc, (xCoord + 1) * cxClient / 3, (yCoord + 1) * cyClient / 3);
						MoveToEx(hdc, (xCoord + 1) * cxClient / 3, 20 + yCoord * cyClient / 3, NULL);
						LineTo(hdc, 20 + xCoord * cxClient / 3, (yCoord + 1) * cyClient / 3);
					}
					if (pLoc->GetSymbol() == 0)
					{
						Ellipse(hdc, 20 + xCoord * cxClient / 3, 20 + yCoord * cyClient / 3, 
							    (xCoord + 1) * cxClient / 3, (yCoord + 1) * cyClient / 3);
					}
					DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
				}
				pLoc++;
			}
			if (gameOver)
			{
				HPEN hpen = CreatePen(PS_SOLID, 5, RGB(255,0,0));
				SelectObject(hdc, hpen);
				switch (winType)
				{
				case 1:
					MoveToEx(hdc, 0, 10 + cyClient / 6, NULL);
					LineTo(hdc, cxClient + 20, 10 + cyClient / 6);
					break;
				case 2:
					MoveToEx(hdc, 10 + cxClient / 6, 0, NULL);
					LineTo(hdc, 10 + cxClient / 6, 20 + cyClient);
					break;
				case 3:
					MoveToEx(hdc, 0, 0, NULL);
					LineTo(hdc, 20 + cxClient, 20 + cyClient);
					break;
				case 4:
					MoveToEx(hdc, 10 + cxClient / 2, 0, NULL);
					LineTo(hdc, 10 + cxClient / 2, 20 + cyClient);
					break;
				case 5:
					MoveToEx(hdc, 10 + 5 * cxClient / 6, 0, NULL);
					LineTo(hdc, 10 + 5 * cxClient / 6, 20 + cyClient);
					break;
				case 6:
					MoveToEx(hdc, 20 + cxClient, 0, NULL);
					LineTo(hdc, 0, 20 + cyClient);
					break;
				case 7:
					MoveToEx(hdc, 0, 10 + cyClient / 2, NULL);
					LineTo(hdc, 20 + cxClient, 10 + cyClient / 2);
					break;
				case 8:
					MoveToEx(hdc, 0, 10 + 5 * cyClient / 6, NULL);
					LineTo(hdc, 20 + cxClient, 10 + 5 * cyClient / 6);
					break;
				default: break;
				}
				DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
			}
			wstringstream ss;
			if (!resetBoard)
			{
				if (gameOver)
				{				
					ss << L"Congratulation ";
					if (turn == 0)
						ss << L"X, you win!";
					else
						ss << L"O, you win!";
					ss << L" Click to continue.";
				}			
				else
				{
					if (validPlay)
					{
						if (numTurns == 9)
						{
							ss << L"It's a Draw. Click to continue.";
							gameOver = true;
						}
						else
						{
							ss << L"Your turn ";
							if (turn == 0)
								ss << L"O, choose a square.";
							else
								ss << L"X, choose a square.";
						}
					}
					else
					{
						ss << L"Can't play there, try again ";
						if (turn == 0)
							ss << L"O...";
						else
							ss << L"X...";
						validPlay = true;
					}
				}
				wstring out = ss.str();
				TextOut(hdc, 5, cyClient + 30, out.c_str(), out.length());
			}
			else
			{
				wstring out = L"New Game: Left Click, Exit: Right Click";
				TextOut(hdc, 5, cyClient + 30, out.c_str(), out.length());
			}
			EndPaint(hwnd, &ps);
			return 0;
		}

	case WM_LBUTTONDOWN:
		if (gameOver)
		{
			if (resetBoard)
			{
				delete[] pElem;
				pElem = new Elem[9];
				pLoc = pElem;
				turn = 0;
				winType = 0;
				resetBoard = false;
				gameOver = false;
				validPlay = true;
				numTurns = 0;
				InvalidateRect(hwnd, NULL, TRUE);
				return 0;
			}
			resetBoard = true;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		xMouse = LOWORD(lParam);
		yMouse = HIWORD(lParam);
		xCoord = (xMouse - 10) / (cxClient / 3);
		yCoord = (yMouse - 10) / (cyClient / 3);
		if (xCoord > 2)
			xCoord = 2;
		if (yCoord > 2)
			yCoord = 2;
		index = yCoord * 3 + xCoord;
		pLoc = pElem;
		for (int i = 0; i < index; i++)
			pLoc++;
		if (!pLoc->GetState())
		{
			pLoc->SetState();
			pLoc->SetSymbol(turn);
			if (turn > 0)
				turn = 0;
			else
				turn = 1;
			numTurns++;
			gameOver = CheckWin(pElem, winType);
		}
		else
			validPlay = false;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONUP:
		if (gameOver)
			PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CheckWin(Elem* pElem, int& winType)
{
	Elem* pOne = pElem;
	Elem* pTwo = pElem;
	Elem* pThree = pElem;
	pTwo++;
	pThree +=2;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 1;
		return true;
	}
	pTwo += 2;
	pThree += 4;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 2;
		return true;
	}
	pTwo++;
	pThree += 2;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 3;
		return true;
	}
	pOne++;
	pThree--;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 4;
		return true;
	}
	pOne++;
	pTwo++;
	pThree++;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 5;
		return true;
	}
	pTwo--;
	pThree -=2;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 6;
		return true;
	}
	pOne++;
	pThree--;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 7;
		return true;
	}
	pOne += 3;
	pTwo += 3;
	pThree += 3;
	if (Compare(pOne, pTwo, pThree))
	{
		winType = 8;
		return true;
	}
	return false;
}

bool Compare(Elem* pOne, Elem* pTwo, Elem* pThree)
{
	if (pOne->GetState() && pTwo->GetState() && pThree->GetState())
	{
		if (pOne->GetSymbol() == pTwo->GetSymbol() && pOne->GetSymbol() == pThree->GetSymbol())
			return true;
	}
	return false;
}