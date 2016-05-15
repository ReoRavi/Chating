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
// Info : 초기화
BOOL CScene_CreateRoom::Initialize(VOID)
{
	CCreateRoomUI::DrawAllSection();

	bWaiting = FALSE;

	return TRUE;
}

//===============================================================
// Process
// Info : 루프
VOID CScene_CreateRoom::Process(VOID)
{
	// 생성 요청을 하고 대기중이 아니라면,
	if (!bWaiting)
	{
		// 키보드 입력이 있었다면
		if (kbhit())
		{
			// 만약 ESC키가 눌렸다면.
			if (getch() == KEY_ESCAPE)
			{
				// 로비로 돌아간다.
				g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
			}
			else
			{
				// 방의 이름을 받고, 서버로 생성 요청을 한다.
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
// Info : 메모리 해제
BOOL CScene_CreateRoom::Release(VOID)
{
	return TRUE;
}

//===============================================================
// RequestResult
// Info : 서버로부터 방 생성 요청의 성공여부를 받고 처리한다.
// bSuccess - 생성 요청 성공 여부
VOID CScene_CreateRoom::RequestResult(BOOL bSuccess)
{
	// 성공했다면
	if (bSuccess)
	{
		// 씬을 방으로 바꾼다.
		g_pSceneManager->ChangeScene(eSceneType::eScene_Room);
	}
	else
	{
		// 실패를 알려준다.
		CCreateRoomUI::DrawNotice("방 만들기에 실패했습니다.");
	}

	bWaiting = FALSE;
}