#include "stdafx.h"
#include "User.h"


CUser::CUser()
{
}


CUser::~CUser()
{
}

//=================================================
// Initialize
// Info : 초기화
BOOL CUser::Initialize(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	// PacketHandler 초기화
	if (!CPacketHandler::Initialize())
	{
		CLog::WriteLogWithDate("CUser : Initialize - CPacketHandler::Initialize() Error!\n");

		return FALSE;
	}

	userName.clear();

	userState = eUserState::eUserState_NULL;

	bConnected = FALSE;

	heartBeat = 0;

	return TRUE;
}

//=================================================
// Release
// Info : 메모리 해제
BOOL CUser::Release(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	connectedRoom = nullptr;

	if (!CPacketHandler::Release())
	{
		CLog::WriteLogWithDate("CUser : Release - CPacketHandler::Release() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// Restore
// Info : User의 사용이 끝나 다시 접속을 받을 수 있도록 복구한다.
// listenSocket - 실제 서버 소켓, 다시 접속요청을 받을 수 있도록 설정하는데 쓰인다.
BOOL CUser::Restore(SOCKET listenSocket)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	if (!Release())
	{
		CLog::WriteLogWithDate("CUser : Restore - Release() Error!\n");

		return FALSE;
	}

	// 초기화 + TCP소켓 사용 + 접속 받기.
	return Initialize() && TCPSocket() && Accept(listenSocket);
}

//=================================================
// HeartBeatCheck
// Info : 하트비트가 최댓치를 넘어가면 클라이언트에 이상이 생겼다고 판단, 접속을 종료한다.
// listenSocket - 실제 서버 소켓, 다시 접속요청을 받을 수 있도록 설정하는데 쓰인다.
VOID CUser::HeartBeatCheck(SOCKET listenSocket)
{	
	// 임계영역 처리
	CAutoCriticalSection section;

	// 하트비트 최댓치를 넘었다면
	if (heartBeat > MAX_HEARTBEAT)
		Restore(listenSocket);
	else
		heartBeat++;
}

//=================================================
// GetSectionUserName
// Info : 유저의 이름을 리턴한다.
std::string CUser::GetSectionUserName(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	return userName;
}

//=================================================
// SetSectionUserName
// Info : 유저의 이름을 설정한다.
// userName - 설정할 유저의 이름
VOID CUser::SetSectionUserName(std::string userName)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	this->userName = userName;
}

//=================================================
// GetUserState
// Info : 유저의 상태를 리턴한다.
eUserState CUser::GetUserState(VOID)
{
	CAutoCriticalSection section;

	return userState;
}

//=================================================
// SetUserState
// Info : 유저의 상태를 설정한다.
// userState - 설정할 유저의 상태.
VOID CUser::SetUserState(eUserState userState)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	this->userState = userState;
}

//=================================================
// GetConnectedRoom
// Info : 연결된 방을 리턴한다.
CRoom* CUser::GetConnectedRoom(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	return connectedRoom;
}

//=================================================
// SetConnectedRoom
// Info : 연결된 방을 설정한다.
// room - 설정할 방의 포인터
VOID CUser::SetConnectedRoom(CRoom* room)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	connectedRoom = room;
}

//=================================================
// GetConnected
// Info : 연결여부를 리턴한다.
BOOL CUser::GetConnected(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	return bConnected;
}

//=================================================
// SetConnected
// Info : 연결 여부를 설정한다.
// connected - 설정할 연결 여부
VOID CUser::SetConnected(BOOL connected)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	bConnected = connected;
}

//=================================================
// GetHeartBeat
// Info : 하트비트를 리턴한다.
INT CUser::GetHeartBeat(VOID)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	return heartBeat;
}

//=================================================
// SetHeartBeat
// Info : 하트비트를 설정한다.
// count - 설정할 하트비트 카운트
VOID CUser::SetHeartBeat(INT count)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	heartBeat = count;
}

