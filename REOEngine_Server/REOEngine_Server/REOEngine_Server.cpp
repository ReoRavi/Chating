// REOEngine_Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int main()
{
	WSADATA					wsaData;

	// 윈도우 소켓 프로그래밍 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		CLog::WriteLog("main : WSAStartUp() Error!\n");
		return FALSE;
	}
	
	// 로그 초기화
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

