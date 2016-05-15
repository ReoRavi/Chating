#include "stdafx.h"
#include "Scene_CreateRoom.h"

CScene_CreateRoom::CScene_CreateRoom()
{
}


CScene_CreateRoom::~CScene_CreateRoom()
{
}

//===============================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CScene_CreateRoom::Initialize(VOID)
{
	CCreateRoomUI::DrawAllSection();

	bWaiting = FALSE;

	return TRUE;
}

//===============================================================
// Process
// Info : ����
VOID CScene_CreateRoom::Process(VOID)
{
	// ���� ��û�� �ϰ� ������� �ƴ϶��,
	if (!bWaiting)
	{
		// Ű���� �Է��� �־��ٸ�
		if (kbhit())
		{
			// ���� ESCŰ�� ���ȴٸ�.
			if (getch() == KEY_ESCAPE)
			{
				// �κ�� ���ư���.
				g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
			}
			else
			{
				// ���� �̸��� �ް�, ������ ���� ��û�� �Ѵ�.
				std::string roomName;

				getline(std::cin, roomName);

				BYTE buffer[MAX_BUFFER_LENGTH] = { 0, };

				g_pNetwork->WritePacket(
					PT_WRITE_CREATEROOM_REQUEST(buffer, roomName),
					ePACKET_PROTOCOL::ePacket_CreateRoom_Request,
					buffer);

				bWaiting = TRUE;
			}
		}
	}
}

//===============================================================
// Release
// Info : �޸� ����
BOOL CScene_CreateRoom::Release(VOID)
{
	return TRUE;
}

//===============================================================
// RequestResult
// Info : �����κ��� �� ���� ��û�� �������θ� �ް� ó���Ѵ�.
// bSuccess - ���� ��û ���� ����
VOID CScene_CreateRoom::RequestResult(BOOL bSuccess)
{
	// �����ߴٸ�
	if (bSuccess)
	{
		// ���� ������ �ٲ۴�.
		g_pSceneManager->ChangeScene(eSceneType::eScene_Room);
	}
	else
	{
		// ���и� �˷��ش�.
		CCreateRoomUI::DrawNotice("�� ����⿡ �����߽��ϴ�.");
	}

	bWaiting = FALSE;
}