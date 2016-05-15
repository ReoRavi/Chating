#include "stdafx.h"
#include "Lobby.h"

CLobby::CLobby()
{
}


CLobby::~CLobby()
{
}

//=================================================
// Initialize
// Info : 초기화
BOOL	CLobby::Initialize(VOID)
{
	roomManager = new CRoomManager();

	if (!roomManager->Initialize())
	{
		CLog::WriteLog("CLobby : Initialize - roomManager->Initialize() Error!\n");

		return FALSE;
	}

	userCount = 0;

	return TRUE;
}

//=================================================
// Release
// Info : 메모리 해제
BOOL	CLobby::Release(VOID)
{
	if (!roomManager->Release())
	{
		CLog::WriteLog("CLobby : Release - roomManager->Release() Error!\n");

		return FALSE;
	}

	users.clear();

	return TRUE;
}

//=================================================
// GetRoomManager
// Info : 방 매니저를 리턴한다.
CRoomManager* CLobby::GetRoomManager(VOID)
{
	return roomManager;
}

//=================================================
// SetRoomManager
// Info : 방 매니저를 설정한다.
// roomManager - 설정할 CRoomManager의 포인터.
VOID CLobby::SetRoomManager(CRoomManager* roomManager)
{
	// 사용되지 않을 예정
	//this->roomManager = roomManager;
}

//=================================================
// GetUserCount
// Info : 유저 카운트를 리턴한다.
INT CLobby::GetUserCount(VOID)
{
	return userCount;
}

//=================================================
// SetUserCount
// Info : 유저 카운트를 설정한다.
// count - 설정할 userCount의 값.
VOID CLobby::SetUserCount(INT count)
{
	userCount = count;
}

//=================================================
// ConnectToLobby
// Info : 유저가 로비에 접속한다.
// connectedUser - 로비에 접속할려는 유저의 포인터.
BOOL CLobby::ConnectToLobby(CUser* connectedUser)
{
	// 유저의 상태가 이미 로비일 경우에 FALSE를 리턴한다.
	if (connectedUser->GetUserState() == eUserState::eUserState_Lobby)
	{
		CLog::WriteLogWithDate("CLobby : ConnectToLobby - User already in Lobby\n");

		return FALSE;
	}

	// 유저 로비 접속 처리
	connectedUser->SetUserState(eUserState::eUserState_Lobby);

	users.push_back(connectedUser);
	userCount++;

	return TRUE;
}

//=================================================
// DisconnectToLobby
// Info : 유저가 로비에서 접속을 해제한다.
// disconnectedUser - 접속을 해제하려는 유저의 포인터.
BOOL CLobby::DisconnectToLobby(CUser* disconnectedUser)
{
	// 유저를 찾기 위한 변수. List 의 반복자. 
	std::list<CUser*>::iterator user;

	// 접속을 해제하려는 유저를 찾는다.
	user = std::find(users.begin(), users.end(), disconnectedUser);

	// 유저를 찾지 못했음.
	if (*user == NULL)
	{
		CLog::WriteLogWithDate("CLobby : DisconnectToLobby - User not found\n");

		return FALSE;
	}

	// List에서 유저를 지운다.
	users.erase(user);

	userCount--;

	return TRUE;
}

//=================================================
// CreateRoom
// Info : 방을 생성한다.
// host - 방의 방장
// roomName - 만들 방의 이름
BOOL CLobby::CreateRoom(CUser* host, std::string roomName)
{
	// 방을 생성함. 방이 꽉 차있을 경우 생성 불가.
	if (!roomManager->CreateRoom(host, roomName))
	{
		CLog::WriteLogWithDate("CLobby : CreateRoom - Can't create Room\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// SendToAll
// Info : 로비안의 모든 유저에게 패킷을 전송한다.
// length - 패킷의 길이
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
BOOL CLobby::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 모든 유저들에게 패킷을 전송한다.
	for each (CUser* user in users)
	{
		if (!user->WritePacket(length, protocol, buffer))
		{
			CLog::WriteLogWithDate("CLobby : SendToAll - WritePacket() Error!\n");

			return FALSE;
		}
	}

	// 패킷이 모두 잘 전송되었음.
	return TRUE;
}
