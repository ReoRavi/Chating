#include "stdafx.h"
#include "Scene_Lobby.h"


CScene_Lobby::CScene_Lobby()
{
}


CScene_Lobby::~CScene_Lobby()
{
}

//===============================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CScene_Lobby::Initialize(VOID)
{
	cursorIndex = 0;

	// ���۰� ���ÿ� �κ� ������ �޾ƿ��� ���� ��Ŷ�� �����Ѵ�.
	g_pNetwork->WritePacket(0, ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate, NULL);

	return TRUE;
}

//===============================================================
// Process
// Info : ����
VOID CScene_Lobby::Process(VOID)
{
	// Ű���尡 �ԷµǸ�
	if (kbhit())
	{
		switch (getch())
		{
			// ���� ����Ű ó��
		case KEY_UP:
			// Ŀ���� ���� �ø���.
			if (--cursorIndex < 0)
			{
				CLobbyUI::EraseCursor(cursorIndex, 1);
				cursorIndex = MAX_ROOMCOUNT;
			}
			else
				CLobbyUI::EraseCursor(cursorIndex, 1);

			CLobbyUI::DrawCursor(cursorIndex);

			break;

			// �Ʒ��� ����Ű ó��
		case KEY_DOWN:
			// Ŀ���� ������ ������.
			if (++cursorIndex > MAX_ROOMCOUNT)
			{
				CLobbyUI::EraseCursor(cursorIndex, -1);
				cursorIndex = 0;
			}
			else
				CLobbyUI::EraseCursor(cursorIndex, -1);

			CLobbyUI::DrawCursor(cursorIndex);

			break;

			// ����Ű ó��
		case KEY_ENTER:
			BYTE buffer[MAX_BUFFER_LENGTH];

			// ������ �ε����� �濡 ���� ��û ��Ŷ�� ������.
			g_pNetwork->WritePacket(PT_WRITE_LOBBY_JOINROOM(buffer, cursorIndex),
				ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom,
				buffer);

			break;

			// AŰ ó��
		case 'a':
		case 'A':
		case '��':
			// AŰ�� ������ �� ���� ������ �̵��Ѵ�.
			g_pSceneManager->ChangeScene(eSceneType::eScene_CreateRoom);

			return;

			// PŰ ó��
		case 'p' :
		case '��':
			// ������ ���߰� �����Ѵ�.
			g_pSceneManager->run = FALSE;

			return;

		default:
			break;
		}
	}
}

//===============================================================
// Release
// Info : �޸� ����
BOOL CScene_Lobby::Release(VOID)
{
	return TRUE;
}