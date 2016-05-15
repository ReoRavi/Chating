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
// Info : 씬마다 다른 UI 그리기를 처리한다.
// x - X 좌표
// y - Y 좌표
VOID CRoomUI::DrawUI(int x, int y)
{
	gotoxy(x, y);

	if (x == 0)
	{
		if (y == 0)									printf("┏");
		else if (y == INPUT_YPOS)	printf("┣");
		else if (y == MAX_YPOS)		printf("┗");
		else											printf("┃");
	}
	else if (x == MAX_XPOS)
	{
		if (y == 0)									printf("┓");
		else if (y == INPUT_YPOS)	printf("┫");
		else if (y == MAX_YPOS)		printf("┛");
		else											printf("┃");
	}
	else
	{
		if (y == 0 || y == INPUT_YPOS || y == MAX_YPOS)		printf("━");
		else																						printf("  ");

	}
}

//===============================================================
// DrawAllSection
// Info : 씬의 모든 부분을 그린다.
VOID CRoomUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawChatSection();
	DrawInputSection();
}

//===============================================================
// DrawChatSection
// Info : 체팅 출력 부분을 그린다.
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
// Info : 체팅 입력 부분을 그린다.
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
// Info : 알림을 로그에 등록한다.
// notice - 등록할 알림
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
// Info : 체팅을 로그에 등록한다.
// message - 로그에 등록할 체팅
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
// Info : 체팅을 그린다.
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