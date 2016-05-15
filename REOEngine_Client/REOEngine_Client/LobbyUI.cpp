#include "stdafx.h"
#include "LobbyUI.h"


CLobbyUI::CLobbyUI()
{
}


CLobbyUI::~CLobbyUI()
{
}

//===============================================================
// DrawUI
// Info : 씬마다 다른 UI 그리기를 처리한다.
// x - X 좌표
// y - Y 좌표
VOID CLobbyUI::DrawUI(int x, int y)
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
	else if (x == HOSTNAME_XPOS)
	{
		if (y == 0)													printf("┳");
		else if (y == STATEUI_YPOS)				printf("╋");
		else if (y == MAX_YPOS)						printf("┻");
		else															printf("┃");
	}
	else if (x == ROOMSTATE_XPOS)
	{
		if (y == 0)													printf("┳");
		else if (y == STATEUI_YPOS)				printf("╋");
		else if (y == MAX_YPOS)						printf("┻");
		else															printf("┃");
	}
	else if (x == CURSOR_XPOS)
	{
		if (y == 0)													printf("┳");
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
		if (y == 0 || y == STATEUI_YPOS  || y == MAX_YPOS)		printf("━");
		else													printf("  ");
	}
}

//===============================================================
// DrawAllSection
// Info : 씬의 모든 부분을 그린다.
VOID CLobbyUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawLobbySection();
	DrawLobbyUI();
}

//===============================================================
// DrawAllSection
// Info : 씬의 모든 부분을 그린다.
VOID CLobbyUI::DrawLobbySection(VOID)
{
	for (int x = 0; x <= MAX_XPOS; x += 2)
	{
		for (int y = 0; y <= MAX_YPOS; y++)
		{
			DrawUI(x, y);
		}
	}
}

//===============================================================
// DrawLobbyUI
// Info : 로비의 UI 메뉴를 그린다.
VOID CLobbyUI::DrawLobbyUI(VOID)
{
	// UI의 글자수와 안의 내용의 글자수가 다를 경우에는 gotoxy 문이 다름.
	std::string	LobbyUI[MAX_LOBBYUICOUNT] = { "No.", "Host Name", " Room Info", "◀" };

	gotoxy(ROOMINDEX_XPOS - LobbyUI[0].length(), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[0].c_str());
	gotoxy(HOSTNAME_XPOS / 2, STATEUI_YPOS - 1);
	printf("%s", LobbyUI[1].c_str());
	gotoxy(ROOMSTATE_XPOS / 2 + (LobbyUI[2].length() / 2), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[2].c_str());
	gotoxy(CURSOR_XPOS - LobbyUI[3].length(), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[3].c_str());

	std::string LobbyInfo[MAX_LOBBYUICOUNT] = {
	"A : 방 만들기",
	"Enter : 방 들어가기 ",
	"방향키 : 커서 위치 설정",
	"P : 종료" 
	};

	for (int index = 0; index < MAX_LOBBYUICOUNT; index++)
	{
		gotoxy((MAX_XPOS - (CURSOR_XPOS / 2)) - (LobbyInfo[index].length() / 2) + 6, 15 + index);
		printf("%s", LobbyInfo[index].c_str());
	}
}

//===============================================================
// DrawRoomInfo
// Info : 로비의 UI들을 그린다.
// roomCount - 방의 갯수
// roomIndex - 방의 번호들
// roomHost - 방의 호스트의 이름
// roomName - 방의 이름
VOID CLobbyUI::DrawRoomInfo(INT roomCount, INT roomIndex[], std::string roomHost[], std::string roomName[])
{
	for (int index = 0; index < roomCount; index++)
	{
		gotoxy(ROOMINDEX_XPOS - 3, STATEUI_YPOS + index + 1);
		printf("%d", roomIndex[index]);
		gotoxy(HOSTNAME_XPOS / 2 + roomHost[index].length() / 2, STATEUI_YPOS + index + 1);
		printf("%s", roomHost[index].c_str());
		gotoxy((ROOMSTATE_XPOS / 2) + (HOSTNAME_XPOS / 2) - roomName[index].length() / 2, STATEUI_YPOS + index + 1);
		printf("%s", roomName[index].c_str());
	}
}

//===============================================================
// DrawCursor
// Info : 커서를 그린다.
// cursorIndex - 커서의 인덱스
VOID CLobbyUI::DrawCursor(INT cursorIndex)
{
	std::string cursor = "◁";

	gotoxy(CURSOR_XPOS - cursor.length(), CURSOR_YPOS + cursorIndex);
	printf("%s", cursor.c_str());

}

//===============================================================
// EraseCursor
// Info : 이전 커서를 지운다.
VOID CLobbyUI::EraseCursor(INT cursorIndex, INT moveDirection)
{
	std::string cursor = "  ";

	gotoxy(CURSOR_XPOS - cursor.length(), CURSOR_YPOS + cursorIndex + moveDirection);
	printf("  ");
}
