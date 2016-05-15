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
// Info : Ŀ���� �̵���Ų��.
// x - X��ǥ
// y - Y��ǥ
VOID CUI::gotoxy(int x, int y)
{
	COORD Cur = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

//===============================================================
// DrawUIBox
// Info : ���� ū UI �ڽ��� �׸���.
VOID CUI::DrawUIBox(VOID)
{
	for (int x = 0; x <= MAX_XPOS; x += 2)
	{
		for (int y = 0; y <= MAX_YPOS; y++)
		{
			gotoxy(x, y);

			if (x == 0)
			{
				if (y == 0)									printf("��");
				else if (y == MAX_YPOS)		printf("��");
				else											printf("��");
			}
			else if (x == MAX_XPOS)
			{
				if (y == 0)									printf("��");
				else if (y == MAX_YPOS)		printf("��");
				else											printf("��");
			}
			else
			{
				if (y == 0 || y == MAX_YPOS)		printf("��");
				else													printf("  ");
			}
		}
	}
}
