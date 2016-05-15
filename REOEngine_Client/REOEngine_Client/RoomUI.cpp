#include "stdafx.h"
#include "RoomUI.h"


CRoomUI::CRoomUI()
{
	chatingCount = 0;
}


CRoomUI::~CRoomUI()
{
}

//===============================================================
// DrawUI
// Info : ������ �ٸ� UI �׸��⸦ ó���Ѵ�.
// x - X ��ǥ
// y - Y ��ǥ
VOID CRoomUI::DrawUI(int x, int y)
{
	gotoxy(x, y);

	if (x == 0)
	{
		if (y == 0)									printf("��");
		else if (y == INPUT_YPOS)	printf("��");
		else if (y == MAX_YPOS)		printf("��");
		else											printf("��");
	}
	else if (x == MAX_XPOS)
	{
		if (y == 0)									printf("��");
		else if (y == INPUT_YPOS)	printf("��");
		else if (y == MAX_YPOS)		printf("��");
		else											printf("��");
	}
	else
	{
		if (y == 0 || y == INPUT_YPOS || y == MAX_YPOS)		printf("��");
		else																						printf("  ");

	}
}

//===============================================================
// DrawAllSection
// Info : ���� ��� �κ��� �׸���.
VOID CRoomUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawChatSection();
	DrawInputSection();
}

//===============================================================
// DrawChatSection
// Info : ü�� ��� �κ��� �׸���.
VOID CRoomUI::DrawChatSection(VOID)
{
	CAutoCriticalSection section;

	for (int x = 0; x <= MAX_XPOS; x += 2)
	{
		for (int y = 0; y <= INPUT_YPOS; y++)
		{
			DrawUI(x, y);
		}
	}

	gotoxy(3, INPUT_YPOS + 2);
}

//===============================================================
// DrawInputSection
// Info : ü�� �Է� �κ��� �׸���.
VOID CRoomUI::DrawInputSection(VOID)
{
	CAutoCriticalSection section;

	for (int x = 0; x <= MAX_XPOS; x += 2)
	{
		for (int y = INPUT_YPOS; y <= MAX_YPOS; y++)
		{
			DrawUI(x, y);
		}
	}

	gotoxy(3, INPUT_YPOS + 2);
}

//===============================================================
// InputNotice
// Info : �˸��� �α׿� ����Ѵ�.
// notice - ����� �˸�
VOID CRoomUI::InputNotice(std::string notice)
{
	CAutoCriticalSection section;

	std::string inputNotice;

	INT num = notice.length();

	for (int index = 0; index < (MAX_XPOS / 2) - num / 2; index++)
	{
		inputNotice += " ";
	}

	inputNotice += notice;

	InputChating(inputNotice);
}

//===============================================================
// InputChating
// Info : ü���� �α׿� ����Ѵ�.
// message - �α׿� ����� ü��
VOID CRoomUI::InputChating(std::string message)
{
	CAutoCriticalSection section;

	for (int index = chatingCount; index > 0; index--)
	{
		chatingLog[index] = chatingLog[index - 1];
	}

	chatingLog[0] = message;

	if (chatingCount <= MAX_CHATING - 1)
	{
		chatingCount++;
	}
}

//===============================================================
// DrawChating
// Info : ü���� �׸���.
VOID CRoomUI::DrawChating(VOID)
{
	CAutoCriticalSection section;

	for (int index = 0; index < chatingCount; index++)
	{
		gotoxy(3, 1 + index);
		printf("%s", chatingLog[index].c_str());
	}

	gotoxy(3, INPUT_YPOS + 2);
}