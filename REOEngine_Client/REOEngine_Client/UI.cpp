#include "stdafx.h"
#include "UI.h"


CUI::CUI()
{
}


CUI::~CUI()
{
}

//===============================================================
// gotoxy
// Info : 커서를 이동시킨다.
// x - X좌표
// y - Y좌표
VOID CUI::gotoxy(int x, int y)
{
	COORD Cur = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

//===============================================================
// DrawUIBox
// Info : 겉의 큰 UI 박스를 그린다.
VOID CUI::DrawUIBox(VOID)
{
	for (int x = 0; x <= MAX_XPOS; x += 2)
	{
		for (int y = 0; y <= MAX_YPOS; y++)
		{
			gotoxy(x, y);

			if (x == 0)
			{
				if (y == 0)									printf("┏");
				else if (y == MAX_YPOS)		printf("┗");
				else											printf("┃");
			}
			else if (x == MAX_XPOS)
			{
				if (y == 0)									printf("┓");
				else if (y == MAX_YPOS)		printf("┛");
				else											printf("┃");
			}
			else
			{
				if (y == 0 || y == MAX_YPOS)		printf("━");
				else													printf("  ");
			}
		}
	}
}
