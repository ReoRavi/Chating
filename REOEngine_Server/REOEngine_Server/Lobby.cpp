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
// Info : �ʱ�ȭ
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
// Info : �޸� ����
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
// Info : �� �Ŵ����� �����Ѵ�.
CRoomManager* CLobby::GetRoomManager(VOID)
{
	return roomManager;
}

//=================================================
// SetRoomManager
// Info : �� �Ŵ����� �����Ѵ�.
// roomManager - ������ CRoomManager�� ������.
VOID CLobby::SetRoomManager(CRoomManager* roomManager)
{
	// ������ ���� ����
	//this->roomManager = roomManager;
}

//=================================================
// GetUserCount
// Info : ���� ī��Ʈ�� �����Ѵ�.
INT CLobby::GetUserCount(VOID)
{
	return userCount;
}

//=================================================
// SetUserCount
// Info : ���� ī��Ʈ�� �����Ѵ�.
// count - ������ userCount�� ��.
VOID CLobby::SetUserCount(INT count)
{
	userCount = count;
}

//=================================================
// ConnectToLobby
// Info : ������ �κ� �����Ѵ�.
// connectedUser - �κ� �����ҷ��� ������ ������.
BOOL CLobby::ConnectToLobby(CUser* connectedUser)
{
	// ������ ���°� �̹� �κ��� ��쿡 FALSE�� �����Ѵ�.
	if (connectedUser->GetUserState() == eUserState::eUserState_Lobby)
	{
		CLog::WriteLogWithDate("CLobby : ConnectToLobby - User already in Lobby\n");

		return FALSE;
	}

	// ���� �κ� ���� ó��
	connectedUser->SetUserState(eUserState::eUserState_Lobby);

	users.push_back(connectedUser);
	userCount++;

	return TRUE;
}

//=================================================
// DisconnectToLobby
// Info : ������ �κ񿡼� ������ �����Ѵ�.
// disconnectedUser - ������ �����Ϸ��� ������ ������.
BOOL CLobby::DisconnectToLobby(CUser* disconnectedUser)
{
	// ������ ã�� ���� ����. List �� �ݺ���. 
	std::list<CUser*>::iterator user;

	// ������ �����Ϸ��� ������ ã�´�.
	user = std::find(users.begin(), users.end(), disconnectedUser);

	// ������ ã�� ������.
	if (*user == NULL)
	{
		CLog::WriteLogWithDate("CLobby : DisconnectToLobby - User not found\n");

		return FALSE;
	}

	// List���� ������ �����.
	users.erase(user);

	userCount--;

	return TRUE;
}

//=================================================
// CreateRoom
// Info : ���� �����Ѵ�.
// host - ���� ����
// roomName - ���� ���� �̸�
BOOL CLobby::CreateRoom(CUser* host, std::string roomName)
{
	// ���� ������. ���� �� ������ ��� ���� �Ұ�.
	if (!roomManager->CreateRoom(host, roomName))
	{
		CLog::WriteLogWithDate("CLobby : CreateRoom - Can't create Room\n");

		return FALSE;
	}

	return TRUE;
}

//=================================================
// SendToAll
// Info : �κ���� ��� �������� ��Ŷ�� �����Ѵ�.
// length - ��Ŷ�� ����
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
BOOL CLobby::SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// ��� �����鿡�� ��Ŷ�� �����Ѵ�.
	for each (CUser* user in users)
	{
		if (!user->WritePacket(length, protocol, buffer))
		{
			CLog::WriteLogWithDate("CLobby : SendToAll - WritePacket() Error!\n");

			return FALSE;
		}
	}

	// ��Ŷ�� ��� �� ���۵Ǿ���.
	return TRUE;
}
