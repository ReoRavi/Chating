#include "stdafx.h"
#include "CreateRoomUI.h"


CCreateRoomUI::CCreateRoomUI()
{
}


CCreateRoomUI::~CCreateRoomUI()
{
}

//===============================================================
// DrawUI
// Info : 씬마다 다른 UI 그리기를 처리한다.
// x - X 좌표
// y - Y 좌표
VOID CCreateRoomUI::DrawUI(int x, int y)
{
	gotoxy(x, y);

	if (x == 0)
	{
		if (y == 0)													printf("┏");
		else if (y == STATEUI_YPOS)				printf("┣");
		else if (y == MAX_YPOS)						printf("┗");
		else															printf("┃");
	}
	else if (x == ROOMINDEX_XPOS)
	{
		if (y == 0)													printf("┳");
		else if (y == STATEUI_YPOS)				printf("╋");
		else if (y == STATEUI_YPOS)				printf("╋");
		else if (y == MAX_YPOS)						printf("┻");
		else															printf("┃");
	}
	else if (x == MAX_XPOS)
	{
		if (y == 0)													printf("┓");
		else if (y == STATEUI_YPOS)				printf("┫");
		else if (y == MAX_YPOS)						printf("┛");
		else															printf("┃");
	}
	else
	{
		if (y == 0 || y == MAX_YPOS)		printf("━");
		else													printf("  ");
	}
}

//===============================================================
// DrawAllSection
// Info : 씬의 모든 부분을 그린다.
VOID CCreateRoomUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawUIBox();
	DrawSectionUI();
}

//===============================================================
// DrawSectionUI
// Info : 씬의 UI를 그린다.
VOID CCreateRoomUI::DrawSectionUI(VOID)
{
	DrawInputBox();

	std::string UI[5] = {
		"Room Name : ",
		"방 이름은 16글자 이내로 가능합니다.",
		"방이 24개 이상일 경우 생성이 불가합니다.",
		"ESC키를 누르면 되돌아갑니다.",
		"입력을 시작하시면 ESC를 눌러도 되돌아 갈 수 없습니다."
	};

	for (int index = 1; index < 5; index++)
	{
		gotoxy(MAX_XPOS / 2 - UI[index].length() / 2, BOX_YPOS + 4 + index);
		printf("%s", UI[index].c_str());
	}

	gotoxy(MAX_XPOS / 2 - UI[0].length() / 2 - 8, BOX_YPOS + 2);
	printf("%s", UI[0].c_str());
}

//===============================================================
// DrawInputBox
// Info : 방 생성의 입력 박스를 그린다.
VOID CCreateRoomUI::DrawInputBox(VOID)
{
	std::string Box[3] = {
		"┌───────────────────────────┐",
		"│                                                      │",
		"└───────────────────────────┘"
	};

	INT boxYPos = BOX_YPOS;

	gotoxy(MAX_XPOS / 2 - Box[0].length() / 2, boxYPos++);
	printf("%s", Box[0].c_str());

	for (int index = 0; index < BOX_SIZE; index++)
	{
		gotoxy(MAX_XPOS / 2 - Box[1].length() / 2, boxYPos++);
		printf("%s", Box[1].c_str());
	}

	gotoxy(MAX_XPOS / 2 - Box[2].length() / 2, boxYPos);
	printf("%s", Box[2].c_str());
}

//===============================================================
// DrawNotice
// Info : 알림을 그린다.
// notice - 그릴 알림
VOID CCreateRoomUI::DrawNotice(std::string notice)
{
	gotoxy(MAX_XPOS / 2 - notice.length() / 2, NOTICE_YPOS);
	printf("%s", notice.c_str());
}
