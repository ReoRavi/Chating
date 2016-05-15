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
// Info : ������ �ٸ� UI �׸��⸦ ó���Ѵ�.
// x - X ��ǥ
// y - Y ��ǥ
VOID CLobbyUI::DrawUI(int x, int y)
{
	gotoxy(x, y);

	if (x == 0)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else if (x == ROOMINDEX_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else if (x == HOSTNAME_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else if (x == ROOMSTATE_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else if (x == CURSOR_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else if (x == MAX_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
	}
	else
	{
		if (y == 0 || y == STATEUI_YPOS  || y == MAX_YPOS)		printf("��");
		else													printf("  ");
	}
}

//===============================================================
// DrawAllSection
// Info : ���� ��� �κ��� �׸���.
VOID CLobbyUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawLobbySection();
	DrawLobbyUI();
}

//===============================================================
// DrawAllSection
// Info : ���� ��� �κ��� �׸���.
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
// Info : �κ��� UI �޴��� �׸���.
VOID CLobbyUI::DrawLobbyUI(VOID)
{
	// UI�� ���ڼ��� ���� ������ ���ڼ��� �ٸ� ��쿡�� gotoxy ���� �ٸ�.
	std::string	LobbyUI[MAX_LOBBYUICOUNT] = { "No.", "Host Name", " Room Info", "��" };

	gotoxy(ROOMINDEX_XPOS - LobbyUI[0].length(), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[0].c_str());
	gotoxy(HOSTNAME_XPOS / 2, STATEUI_YPOS - 1);
	printf("%s", LobbyUI[1].c_str());
	gotoxy(ROOMSTATE_XPOS / 2 + (LobbyUI[2].length() / 2), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[2].c_str());
	gotoxy(CURSOR_XPOS - LobbyUI[3].length(), STATEUI_YPOS - 1);
	printf("%s", LobbyUI[3].c_str());

	std::string LobbyInfo[MAX_LOBBYUICOUNT] = {
	"A : �� �����",
	"Enter : �� ���� ",
	"����Ű : Ŀ�� ��ġ ����",
	"P : ����" 
	};

	for (int index = 0; index < MAX_LOBBYUICOUNT; index++)
	{
		gotoxy((MAX_XPOS - (CURSOR_XPOS / 2)) - (LobbyInfo[index].length() / 2) + 6, 15 + index);
		printf("%s", LobbyInfo[index].c_str());
	}
}

//===============================================================
// DrawRoomInfo
// Info : �κ��� UI���� �׸���.
// roomCount - ���� ����
// roomIndex - ���� ��ȣ��
// roomHost - ���� ȣ��Ʈ�� �̸�
// roomName - ���� �̸�
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
// Info : Ŀ���� �׸���.
// cursorIndex - Ŀ���� �ε���
VOID CLobbyUI::DrawCursor(INT cursorIndex)
{
	std::string cursor = "��";

	gotoxy(CURSOR_XPOS - cursor.length(), CURSOR_YPOS + cursorIndex);
	printf("%s", cursor.c_str());

}

//===============================================================
// EraseCursor
// Info : ���� Ŀ���� �����.
VOID CLobbyUI::EraseCursor(INT cursorIndex, INT moveDirection)
{
	std::string cursor = "  ";

	gotoxy(CURSOR_XPOS - cursor.length(), CURSOR_YPOS + cursorIndex + moveDirection);
	printf("  ");
}
