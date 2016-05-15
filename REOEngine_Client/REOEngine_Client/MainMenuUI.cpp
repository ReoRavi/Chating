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
// Info : 씬마다 다른 UI 그리기를 처리한다.
// x - X 좌표
// y - Y 좌표
VOID CMainMenuUI::DrawUI(int x, int y)
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
		if (y == 0  || y == MAX_YPOS)		printf("━");
		else													printf("  ");
	}
}

//===============================================================
// DrawAllSection
// Info : 씬의 모든 부분을 그린다.
VOID CMainMenuUI::DrawAllSection()
{
	CUI::DrawUIBox();
	DrawMainSectionUI();
	DrawCursor();
}

//===============================================================
// CMainMenuUI
// Info : 메인메뉴의 UI를 그린다.
VOID CMainMenuUI::DrawMainSectionUI()
{
	std::string	titleName = "Chating Program";
	// 길이는 짝수로 맞추는것이 보기가 좋음.
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
// Info : 커서를 그린다.
VOID CMainMenuUI::DrawCursor()
{
	EraseCursor();
	gotoxy(UI_XPOS + CURSOR_GAP, UI_YPOS + cursorState * 2);
	printf("◀◁");
}

//===============================================================
// EraseCursor
// Info : 커서를 지운다
VOID CMainMenuUI::EraseCursor()
{
	for (int index = 0; index < MAIN_UICOUNT; index++)
	{
		gotoxy(UI_XPOS + CURSOR_GAP, UI_YPOS + index * 2);
		printf("    ");
	}
}
