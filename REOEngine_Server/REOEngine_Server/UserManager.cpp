#include "stdafx.h"
#include "UserManager.h"


CUserManager::CUserManager()
{
}


CUserManager::~CUserManager()
{
}

//=================================================
// Initialize
// Info : 초기화
// listenSocket - 연결 요청에 쓰일 서버 소켓.
BOOL CUserManager::Initialize(SOCKET listenSocket)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 최대 유저수만큼 루프를 돌리고, 연결 대기상태로 만든다.
	for (int index = 0; index < MAX_USER; index++)
	{
		CUser* user = new CUser();

		if (!user->Initialize())
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->Initialize() Error!\n");

			return FALSE;
		}

		if (!user->TCPSocket())
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->TCPSocket() Error!\n");

			return FALSE;
		}

		vUsers.push_back(user);
	}

	// 역방향으로 돌리지 않으면 마지막 인자부터 소켓이 할당됨.
	// 유저객체를 연결 요청 대기 상태로 만든다.
	for (int index = MAX_USER - 1; index >= 0; index--)
	{
		if (!vUsers[index]->Accept(listenSocket))
		{
			CLog::WriteLogWithDate("CUserManager : Initialize - user->Accept() Error!\n");

			return FALSE;
		}
	}

	userCount = 0;

	return TRUE;
}

//=================================================
// Release
// Info : 메모리 해제
BOOL CUserManager::Release(VOID)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	for (std::vector<CUser*>::iterator iter = vUsers.begin(); iter != vUsers.end(); ++iter)
	{
		if ((*iter)->Release())
		{
			CLog::WriteLogWithDate("CUserManager : Release - (*iter)->Release() Error!\n");

			return FALSE;
		}

		delete (*iter);
	}

	vUsers.clear();

	return TRUE;
}

//=================================================
// CheckAllHeartBeat
// Info : 모든 유저들의 하트비트를 체크한다.
// listenSocket - 하트비트 처리에 필요한 서버 소켓
VOID CUserManager::CheckAllHeartBeat(SOCKET listenSocket)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 접속해있는 유저들의 하트비트를 체크한다.
	for each (CUser* user in vUsers)
	{
		if (user->GetConnected())
			user->HeartBeatCheck(listenSocket);
	}
}

//=================================================
// SendToAll
// Info : 모든 유저들에게 패킷을 보낸다
// length - 패킷의 길이
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
BOOL CUserManager::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 접속해있는 모든 유저들에게 패킷을 보낸다.
	for each (CUser* user in vUsers)
	{
		if (user->GetConnected())
			if (!user->WritePacket(length, protocol, buffer))
				return FALSE;
	}

	return TRUE;
}

