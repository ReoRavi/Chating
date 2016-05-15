#include "stdafx.h"
#include "MainMenuUI.h"


CMainMenuUI::CMainMenuUI()
{
	cursorState = 0;
}


CMainMenuUI::~CMainMenuUI()
{
}

//===============================================================
// DrawUI
// Info : ������ �ٸ� UI �׸��⸦ ó���Ѵ�.
// x - X ��ǥ
// y - Y ��ǥ
VOID CMainMenuUI::DrawUI(int x, int y)
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
		if (y == 0  || y == MAX_YPOS)		printf("��");
		else													printf("  ");
	}
}

//===============================================================
// DrawAllSection
// Info : ���� ��� �κ��� �׸���.
VOID CMainMenuUI::DrawAllSection()
{
	CUI::DrawUIBox();
	DrawMainSectionUI();
	DrawCursor();
}

//===============================================================
// CMainMenuUI
// Info : ���θ޴��� UI�� �׸���.
VOID CMainMenuUI::DrawMainSectionUI()
{
	std::string	titleName = "Chating Program";
	// ���̴� ¦���� ���ߴ°��� ���Ⱑ ����.
	std::string	UIName[3] = { " Start", "Info", "Exit" };

	gotoxy(TITLE_XPOS - titleName.length() / 2, TITLE_YPOS);
	printf("%s", titleName.c_str());

	for (int index = 0; index < MAIN_UICOUNT; index++)
	{
		gotoxy(UI_XPOS - UIName[index].length() / 2, UI_YPOS + index * 2);
		printf("%s", UIName[index].c_str());
	}
}

//===============================================================
// DrawCursor
// Info : Ŀ���� �׸���.
VOID CMainMenuUI::DrawCursor()
{
	EraseCursor();
	gotoxy(UI_XPOS + CURSOR_GAP, UI_YPOS + cursorState * 2);
	printf("����");
}

//===============================================================
// EraseCursor
// Info : Ŀ���� �����
VOID CMainMenuUI::EraseCursor()
{
	for (int index = 0; index < MAIN_UICOUNT; index++)
	{
		gotoxy(UI_XPOS + CURSOR_GAP, UI_YPOS + index * 2);
		printf("    ");
	}
}
