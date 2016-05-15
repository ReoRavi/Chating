#include "stdafx.h"
#include "Room.h"


CRoom::CRoom()
{
}


CRoom::~CRoom()
{
}

//=================================================
// Initialize
// Info : 초기화
// index - 방의 번호
BOOL CRoom::Initialize(INT index)
{
	userCount = 0;

	roomIndex = index;

	bActive = FALSE;

	return TRUE;
}

//=================================================
// Release
// Info : 메모리 해제
BOOL CRoom::Release(VOID)
{
	if (!users.empty())
		users.clear();

	if (host)
		host = nullptr;

	return TRUE;
}

//=================================================
// GetRoomIndex
// Info : 방의 번호를 리턴함.
INT CRoom::GetRoomIndex(VOID)
{
	return roomIndex;
}

//=================================================
// SetRoomIndex
// Info : 방의 번호를 설정함
// index - 설정할 방의 번호
VOID CRoom::SetRoomIndex(INT index)
{
	roomIndex = index;
}

//=================================================
// GetRoomUserCount
// Info : 방 안의 유저들의 숫자를 리턴함.
INT CRoom::GetRoomUserCount(VOID)
{
	return userCount;
}

//=================================================
// SetRoomUserCount
// Info : 유저들의 숫자를 설정한다.
// count - 설정할 유저들의 숫자.
VOID CRoom::SetRoomUserCount(INT count)
{
	userCount = count;
}

//=================================================
// GetRoomActive
// Info : 방의 활성화 여부를 리턴한다.
BOOL CRoom::GetRoomActive(VOID)
{
	return bActive;
}

//=================================================
// SetRoomActive
// Info : 방의 활성화 여부를 설정한다.
// active - 설정할 방의 활성화 여부.
VOID CRoom::SetRoomActive(BOOL active)
{
	bActive = active;
}

//=================================================
// GetRoomHost
// Info : 방의 방장을 리턴한다.
CUser* CRoom::GetRoomHost(VOID)
{
	return host;
}

//=================================================
// SetRoomHost
// Info : 방의 방장을 설정한다.
// host - 설정할 방의 방장의 포인터.
VOID CRoom::SetRoomHost(CUser* host)
{
	this->host = host;
}

//=================================================
// GetRoomName
// Info : 방의 이름을 리턴한다.
std::string CRoom::GetRoomName(VOID)
{
	return roomName;
}

//=================================================
// SetRoomName
// Info : 방의 이름을 설정한다.
// roomName - 설정할 방의 이름
VOID CRoom::SetRoomName(std::string roomName)
{
	this->roomName = roomName;
}

//=================================================
// ConnectToRoom
// Info : 유저가 방에 접속을 요청함
// connectUser - 접속을 요청한 유저의 포인터
BOOL CRoom::ConnectToRoom(CUser* connectUser)
{
	// 방이 꽉 차서 유저가 들어올 수 없음.
	if (users.size() >= MAX_ROOMUSERS)
	{
		std::string error = "CRoom : ConnectToRoom - ";
		error += roomName + "Room is FULL\n";

		CLog::WriteLogWithDate(error.c_str());

		return FALSE;
	}

	// 유저는 무조건 로비에서 방으로 접속하는 구조를 가지고 있음.
	if (!connectUser->GetUserState() == eUserState::eUserState_Lobby)
		return FALSE;

	// 방 접속 처리
	connectUser->SetUserState(eUserState::eUserState_Room);

	connectUser->SetConnectedRoom(this);

	users.push_back(connectUser);
	userCount++;

	return TRUE;
}

//=================================================
// DisconnectToRoom
// Info : 유저가 방에서 나가는것을 요청함.
// disconnectUser - 나가는것을 요청한 유저의 포인터
BOOL CRoom::DisconnectToRoom(CUser* disconnectUser)
{
	// 나간 유저를 유저 목록에서 검색한다.
	std::list<CUser*>::iterator iter = std::find(users.begin(), users.end(), disconnectUser);

	// 유저를 찾지 못함.
	if (*iter == NULL)
	{
		std::string error = "CRoom : ConnectToRoom - ";
		error += roomName + "disconnected User not Found\n";

		CLog::WriteLogWithDate(error.c_str());

		return FALSE;
	}

	// 유저 나감 처리.
	users.erase(iter);

	userCount--;

	return TRUE;
}

//=================================================
// CloseRoom
// Info : 방을 닫는다.
// lobby - lobby 객체, 방이 닫혀 방에 있던 유저들은 로비에 접속해야함.
BOOL CRoom::CloseRoom(CLobby* lobby)
{
	CRoomManager* roomManager = lobby->GetRoomManager();

	// 방이 하나 삭제됬음으로 카운트를 낮춘다.
	roomManager->SetActiveRoomCount(roomManager->GetActiveRoomCount() - 1);

	// 방안에있는 모든 유저들에게 패킷을 보낸다.
	for each (CUser* user  in users)
	{
		if (!user->WritePacket(0, ePACKET_PROTOCOL::ePacket_Room_Exit, NULL))
		{
			CLog::WriteLogWithDate("CRoom : CloseRoom -  WritePacket() Error!\n");

			return FALSE;
		}

		// 로비에 접속한다.
		if (!lobby->ConnectToLobby(user))
		{
			CLog::WriteLogWithDate("CRoom : CloseRoom -  lobby->ConnectToLobby() Error!\n");

			return FALSE;
		}
	}

	// 닫힌 방을 초기화한다.
	ResetRoom();

	return TRUE;
}

//=================================================
// ResetRoom
// Info : 방을 원래상태로 초기화한다.
BOOL CRoom::ResetRoom(VOID)
{
	// List를 초기화한다.
	users.clear();

	host = NULL;
	userCount = 0;
	bActive = FALSE;

	roomName.clear();

	return TRUE;
}

//=================================================
// SendToAll
// Info : 방 안에있는 모두에게 패킷을 보낸다.
BOOL CRoom::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 방 안의 모든 유저들에게 패킷을 보낸다.
	for each (CUser* user in users)
	{
		if (!user->WritePacket(length, protocol, buffer))
		{
			CLog::WriteLogWithDate("CRoom : SendToAll -  WritePacket() Error!\n");

			return FALSE;
		}
	}

	// 패킷이 모두 잘 전송되었음.
	return TRUE;
}
