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
// Info : 초기화
BOOL CScene_Lobby::Initialize(VOID)
{
	cursorIndex = 0;

	// 시작과 동시에 로비 정보를 받아오기 위해 패킷을 전송한다.
	g_pNetwork->WritePacket(0, ePACKET_PROTOCOL::ePacket_Lobby_InfoUpdate, NULL);

	return TRUE;
}

//===============================================================
// Process
// Info : 루프
VOID CScene_Lobby::Process(VOID)
{
	// 키보드가 입력되면
	if (kbhit())
	{
		switch (getch())
		{
			// 위쪽 방향키 처리
		case KEY_UP:
			// 커서를 위로 올린다.
			if (--cursorIndex < 0)
			{
				CLobbyUI::EraseCursor(cursorIndex, 1);
				cursorIndex = MAX_ROOMCOUNT;
			}
			else
				CLobbyUI::EraseCursor(cursorIndex, 1);

			CLobbyUI::DrawCursor(cursorIndex);

			break;

			// 아래쪽 방향키 처리
		case KEY_DOWN:
			// 커서를 밑으로 내린다.
			if (++cursorIndex > MAX_ROOMCOUNT)
			{
				CLobbyUI::EraseCursor(cursorIndex, -1);
				cursorIndex = 0;
			}
			else
				CLobbyUI::EraseCursor(cursorIndex, -1);

			CLobbyUI::DrawCursor(cursorIndex);

			break;

			// 엔터키 처리
		case KEY_ENTER:
			BYTE buffer[MAX_BUFFER_LENGTH];

			// 지정된 인덱스의 방에 접속 요청 패킷을 보낸다.
			g_pNetwork->WritePacket(PT_WRITE_LOBBY_JOINROOM(buffer, cursorIndex),
				ePACKET_PROTOCOL::ePacket_Lobby_JoinRoom,
				buffer);

			break;

			// A키 처리
		case 'a':
		case 'A':
		case 'ㅁ':
			// A키를 누르면 방 생성 씬으로 이동한다.
			g_pSceneManager->ChangeScene(eSceneType::eScene_CreateRoom);

			return;

			// P키 처리
		case 'p' :
		case 'ㅔ':
			// 루프를 멈추고 종료한다.
			g_pSceneManager->run = FALSE;

			return;

		default:
			break;
		}
	}
}

//===============================================================
// Release
// Info : 메모리 해제
BOOL CScene_Lobby::Release(VOID)
{
	return TRUE;
}