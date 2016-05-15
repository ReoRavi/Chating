// REOEngine_Client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

int main()
{
	WSADATA					wsaData;

	// ������ ���� ���α׷��� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return FALSE;
	}

	// ȭ�� ����
	system("mode con:cols=120 lines=30");
	// �α� �ʱ�ȭ
	CLog::CreateLogFile("log.txt");

	// ��Ʈ��ũ �ʱ�ȭ
	if (!g_pNetwork->Initialize())
	{
		return FALSE;
	}

	// �� �Ŵ��� �ʱ�ȭ
	g_pSceneManager->Initialize();
	// ���θ޴� ������ �ʱ�ȭ
	g_pSceneManager->ChangeScene(eSceneType::eScene_MainMenu);

	// �� ����
	while (g_pSceneManager->run)
	{
		g_pSceneManager->GetCurrentScene()->Process();
	}

	g_pSceneManager->Release();

	g_pNetwork->Release();

    return 0;
}

