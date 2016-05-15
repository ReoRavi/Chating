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
// Info : ������ �ٸ� UI �׸��⸦ ó���Ѵ�.
// x - X ��ǥ
// y - Y ��ǥ
VOID CCreateRoomUI::DrawUI(int x, int y)
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
	else if (x == MAX_XPOS)
	{
		if (y == 0)													printf("��");
		else if (y == STATEUI_YPOS)				printf("��");
		else if (y == MAX_YPOS)						printf("��");
		else															printf("��");
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
VOID CCreateRoomUI::DrawAllSection(VOID)
{
	CAutoCriticalSection section;

	DrawUIBox();
	DrawSectionUI();
}

//===============================================================
// DrawSectionUI
// Info : ���� UI�� �׸���.
VOID CCreateRoomUI::DrawSectionUI(VOID)
{
	DrawInputBox();

	std::string UI[5] = {
		"Room Name : ",
		"�� �̸��� 16���� �̳��� �����մϴ�.",
		"���� 24�� �̻��� ��� ������ �Ұ��մϴ�.",
		"ESCŰ�� ������ �ǵ��ư��ϴ�.",
		"�Է��� �����Ͻø� ESC�� ������ �ǵ��� �� �� �����ϴ�."
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
// Info : �� ������ �Է� �ڽ��� �׸���.
VOID CCreateRoomUI::DrawInputBox(VOID)
{
	std::string Box[3] = {
		"����������������������������������������������������������",
		"��                                                      ��",
		"����������������������������������������������������������"
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
// Info : �˸��� �׸���.
// notice - �׸� �˸�
VOID CCreateRoomUI::DrawNotice(std::string notice)
{
	gotoxy(MAX_XPOS / 2 - notice.length() / 2, NOTICE_YPOS);
	printf("%s", notice.c_str());
}
