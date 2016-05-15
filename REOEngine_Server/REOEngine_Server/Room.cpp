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
// Info : �ʱ�ȭ
// index - ���� ��ȣ
BOOL CRoom::Initialize(INT index)
{
	userCount = 0;

	roomIndex = index;

	bActive = FALSE;

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
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
// Info : ���� ��ȣ�� ������.
INT CRoom::GetRoomIndex(VOID)
{
	return roomIndex;
}

//=================================================
// SetRoomIndex
// Info : ���� ��ȣ�� ������
// index - ������ ���� ��ȣ
VOID CRoom::SetRoomIndex(INT index)
{
	roomIndex = index;
}

//=================================================
// GetRoomUserCount
// Info : �� ���� �������� ���ڸ� ������.
INT CRoom::GetRoomUserCount(VOID)
{
	return userCount;
}

//=================================================
// SetRoomUserCount
// Info : �������� ���ڸ� �����Ѵ�.
// count - ������ �������� ����.
VOID CRoom::SetRoomUserCount(INT count)
{
	userCount = count;
}

//=================================================
// GetRoomActive
// Info : ���� Ȱ��ȭ ���θ� �����Ѵ�.
BOOL CRoom::GetRoomActive(VOID)
{
	return bActive;
}

//=================================================
// SetRoomActive
// Info : ���� Ȱ��ȭ ���θ� �����Ѵ�.
// active - ������ ���� Ȱ��ȭ ����.
VOID CRoom::SetRoomActive(BOOL active)
{
	bActive = active;
}

//=================================================
// GetRoomHost
// Info : ���� ������ �����Ѵ�.
CUser* CRoom::GetRoomHost(VOID)
{
	return host;
}

//=================================================
// SetRoomHost
// Info : ���� ������ �����Ѵ�.
// host - ������ ���� ������ ������.
VOID CRoom::SetRoomHost(CUser* host)
{
	this->host = host;
}

//=================================================
// GetRoomName
// Info : ���� �̸��� �����Ѵ�.
std::string CRoom::GetRoomName(VOID)
{
	return roomName;
}

//=================================================
// SetRoomName
// Info : ���� �̸��� �����Ѵ�.
// roomName - ������ ���� �̸�
VOID CRoom::SetRoomName(std::string roomName)
{
	this->roomName = roomName;
}

//=================================================
// ConnectToRoom
// Info : ������ �濡 ������ ��û��
// connectUser - ������ ��û�� ������ ������
BOOL CRoom::ConnectToRoom(CUser* connectUser)
{
	// ���� �� ���� ������ ���� �� ����.
	if (users.size() >= MAX_ROOMUSERS)
	{
		std::string error = "CRoom : ConnectToRoom - ";
		error += roomName + "Room is FULL\n";

		CLog::WriteLogWithDate(error.c_str());

		return FALSE;
	}

	// ������ ������ �κ񿡼� ������ �����ϴ� ������ ������ ����.
	if (!connectUser->GetUserState() == eUserState::eUserState_Lobby)
		return FALSE;

	// �� ���� ó��
	connectUser->SetUserState(eUserState::eUserState_Room);

	connectUser->SetConnectedRoom(this);

	users.push_back(connectUser);
	userCount++;

	return TRUE;
}

//=================================================
// DisconnectToRoom
// Info : ������ �濡�� �����°��� ��û��.
// disconnectUser - �����°��� ��û�� ������ ������
BOOL CRoom::DisconnectToRoom(CUser* disconnectUser)
{
	// ���� ������ ���� ��Ͽ��� �˻��Ѵ�.
	std::list<CUser*>::iterator iter = std::find(users.begin(), users.end(), disconnectUser);

	// ������ ã�� ����.
	if (*iter == NULL)
	{
		std::string error = "CRoom : ConnectToRoom - ";
		error += roomName + "disconnected User not Found\n";

		CLog::WriteLogWithDate(error.c_str());

		return FALSE;
	}

	// ���� ���� ó��.
	users.erase(iter);

	userCount--;

	return TRUE;
}

//=================================================
// CloseRoom
// Info : ���� �ݴ´�.
// lobby - lobby ��ü, ���� ���� �濡 �ִ� �������� �κ� �����ؾ���.
BOOL CRoom::CloseRoom(CLobby* lobby)
{
	CRoomManager* roomManager = lobby->GetRoomManager();

	// ���� �ϳ� ������������ ī��Ʈ�� �����.
	roomManager->SetActiveRoomCount(roomManager->GetActiveRoomCount() - 1);

	// ��ȿ��ִ� ��� �����鿡�� ��Ŷ�� ������.
	for each (CUser* user  in users)
	{
		if (!user->WritePacket(0, ePACKET_PROTOCOL::ePacket_Room_Exit, NULL))
		{
			CLog::WriteLogWithDate("CRoom : CloseRoom -  WritePacket() Error!\n");

			return FALSE;
		}

		// �κ� �����Ѵ�.
		if (!lobby->ConnectToLobby(user))
		{
			CLog::WriteLogWithDate("CRoom : CloseRoom -  lobby->ConnectToLobby() Error!\n");

			return FALSE;
		}
	}

	// ���� ���� �ʱ�ȭ�Ѵ�.
	ResetRoom();

	return TRUE;
}

//=================================================
// ResetRoom
// Info : ���� �������·� �ʱ�ȭ�Ѵ�.
BOOL CRoom::ResetRoom(VOID)
{
	// List�� �ʱ�ȭ�Ѵ�.
	users.clear();

	host = NULL;
	userCount = 0;
	bActive = FALSE;

	roomName.clear();

	return TRUE;
}

//=================================================
// SendToAll
// Info : �� �ȿ��ִ� ��ο��� ��Ŷ�� ������.
BOOL CRoom::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// �� ���� ��� �����鿡�� ��Ŷ�� ������.
	for each (CUser* user in users)
	{
		if (!user->WritePacket(length, protocol, buffer))
		{
			CLog::WriteLogWithDate("CRoom : SendToAll -  WritePacket() Error!\n");

			return FALSE;
		}
	}

	// ��Ŷ�� ��� �� ���۵Ǿ���.
	return TRUE;
}
