#include "stdafx.h"
#include "LoginUI.h"


CLoginUI::CLoginUI()
{
	UIName[0] = "Server IP : ";
	UIName[1] = "User Name : ";
}


CLoginUI::~CLoginUI()
{
}

//===============================================================
// DrawUI
// Info : ������ �ٸ� UI �׸��⸦ ó���Ѵ�.
// x - X ��ǥ
// y - Y ��ǥ
VOID CLoginUI::DrawUI(int x, int y)
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

//===============================================================
// DrawAllSection
// Info : ���� ��� �κ��� �׸���.
VOID CLoginUI::DrawAllSection(VOID)
{
	CUI::DrawUIBox();
	DrawUISection();
}

//===============================================================
// DrawUISection
// Info : ���� UI�� �׸���.
VOID CLoginUI::DrawUISection(VOID)
{
	for (int index = 0; index < CONNECT_UICOUNT; index++)
	{
		gotoxy(UI_XPOS - UIName[index].length() / 2, UI_YPOS + index * 2);
		printf("%s", UIName[index].c_str());
	}
}

//===============================================================
// DrawMessage
// Info : �޼����� �׸���.
// message - �׸� �޼���.
VOID CLoginUI::DrawMessage(std::string message)
{
	gotoxy((MAX_XPOS / 2) - (MAX_XPOS / 3), (MAX_YPOS / 4) * 3);
	printf("                                                                                                 ");

	gotoxy((MAX_XPOS / 2) - message.length() / 2, (MAX_YPOS / 4) * 3);
	printf("%s", message.c_str());
}