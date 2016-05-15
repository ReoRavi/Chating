// REOEngine_Server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

int main()
{
	WSADATA					wsaData;

	// ������ ���� ���α׷��� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		CLog::WriteLog("main : WSAStartUp() Error!\n");
		return FALSE;
	}
	
	// �α� �ʱ�ȭ
	CLog::CreateLogFile("log.txt");

	CChatServer Server;

	if (!Server.Initialize())
		return FALSE;

	Server.Loop();

	if (!Server.Release())
		return FALSE;

	WSACleanup();

    return 0;
}

