// REOEngine_Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int main()
{
	WSADATA					wsaData;

	// 윈도우 소켓 프로그래밍 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return FALSE;
	}

	// 화면 고정
	system("mode con:cols=120 lines=30");
	// 로그 초기화
	CLog::CreateLogFile("log.txt");

	// 네트워크 초기화
	if (!g_pNetwork->Initialize())
	{
		return FALSE;
	}

	// 씬 매니저 초기화
	g_pSceneManager->Initialize();
	// 메인메뉴 씬으로 초기화
	g_pSceneManager->ChangeScene(eSceneType::eScene_MainMenu);

	// 씬 루프
	while (g_pSceneManager->run)
	{
		g_pSceneManager->GetCurrentScene()->Process();
	}

	g_pSceneManager->Release();

	g_pNetwork->Release();

    return 0;
}

