#include "stdafx.h"
#include "Scene_Login.h"


CScene_Login::CScene_Login()
{
}


CScene_Login::~CScene_Login()
{
}

//===============================================================
// Initialize
// Info : 초기화
BOOL CScene_Login::Initialize(VOID)
{
	CLoginUI::DrawAllSection();

	connectRequest = FALSE;

	requestTime = 0;
	currentTime = 0;
	
	return TRUE;
}

//===============================================================
// Process
// Info : 루프
VOID CScene_Login::Process(VOID)
{
	// 연결 요청을 한 상태라면
	if (connectRequest)
	{
		// 서버와 연결이 됬다면
		if (g_pNetwork->bConnected)
		{
			BYTE buffer[MAX_BUFFER_LENGTH];

			// 유저 정보를 서버에 등록한다.
			g_pNetwork->WritePacket(PT_WRITE_LOGIN_USERINFO(buffer, context[1]),
				ePACKET_PROTOCOL::ePacket_Login_UserInfo, 
				buffer);

			// 로비로 이동한다.
			g_pSceneManager->ChangeScene(eSceneType::eScene_Lobby);
		}

		currentTime = GetTickCount();

		// 10초가 지나면 연결요청을 취소하고 다시 연결한다.
		if (currentTime - requestTime > 10000)
		{
			connectRequest = FALSE;
			CLoginUI::DrawAllSection();
			CLoginUI::DrawMessage("연결에 실패했습니다. 서버 상태를 체크하시거나 다시 입력해주세요.");
		}
	}
	else
	{
		// IP와 유저 이름을 받는다.
		for (int index = 0; index < CONNECT_UICOUNT; index++)
		{
			gotoxy(UI_XPOS + UIName[index].length() / 2, UI_YPOS + index * 2);
			getline(std::cin, context[index]);
		}

		// 컨텍스트에 이상이 없다면
		if (ContextCheck())
		{
			// 서버에 연결요청을 한다.
			if (g_pNetwork->ConnectToServer(context[0].c_str(), DEFAULT_PORT))
			{
				connectRequest = TRUE;

				CLoginUI::DrawMessage("연결 대기중입니다..");

				requestTime = GetTickCount();
				currentTime = GetTickCount();
			}
		}
		else
		{
			CLoginUI::DrawAllSection();
			CLoginUI::DrawMessage("글자 수를 확인해주세요.");
		}
	}
}

//===============================================================
// Release
// Info : 메모리 해제
BOOL CScene_Login::Release(VOID)
{
	return TRUE;
}

//===============================================================
// ContextCheck
// Info : 서버로 보낼 컨텍스트에 이상이 없는지 체크한다.
BOOL CScene_Login::ContextCheck(VOID)
{
	// IP는 16자리를 넘을 수 없음
	if (context[0].length() >= 16)
		return FALSE;

	// 이름은 10자리를 넘을 수 없음
	if (context[1].length() >= 10)
		return FALSE;

	return TRUE;
}