#include "stdafx.h"
#include "RoomManager.h"


CRoomManager::CRoomManager()
{
}


CRoomManager::~CRoomManager()
{
}

//=================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CRoomManager::Initialize()
{
	// �̸� ���� �����صд�.
	for (int index = 0; index < MAX_ROOMCOUNT; index++)
	{
		CRoom*	room = new CRoom();

		if (!room->Initialize(index + 1))
		{
			CLog::WriteLogWithDate("CRoomManager : Initialize - room->Initialize() Error!\n");

			return FALSE;
		}

		vRooms.push_back(room);
	}

	activeRoomCount = 0;

	return TRUE;
}

//=================================================
// Release
// Info : �޸� ����
BOOL CRoomManager::Release()
{
	for (std::vector<CRoom*>::iterator iter = vRooms.begin(); iter != vRooms.end(); ++iter)
	{
		if (!(*iter)->Release())
		{
			CLog::WriteLogWithDate("CRoomManager : Release - (*iter)->Release() Error!\n");

			return FALSE;
		}

		delete (*iter);
	}

	vRooms.clear();

	return TRUE;
}

//=================================================
// GetRoom
// Info : ���� �����Ѵ�.
// index - ���� ���� �ε���
CRoom* CRoomManager::GetRoom(INT index)
{
	return vRooms[index];
}

//=================================================
// SetRoom
// Info : ���� �����Ѵ�
// room - ������ ���� ������
// index - ������ ���� �ε���
VOID CRoomManager::SetRoom(CRoom* room, INT index)
{
	vRooms[index] = room;
}

//=================================================
// GetActiveRoomCount
// Info : Ȱ��ȭ�� ���� ī��Ʈ�� �����Ѵ�.
INT CRoomManager::GetActiveRoomCount()
{
	return activeRoomCount;
}

//=================================================
// SetActiveRoomCount
// Info : Ȱ��ȭ�� ���� ī��Ʈ�� �����Ѵ�.
// count - ������ ī��Ʈ ��
VOID CRoomManager::SetActiveRoomCount(INT count)
{
	activeRoomCount = count;
}

//=================================================
// CreateRoom
// Info : ���� �����.
// host - ������ ������.
// roomName - ���� �̸�
BOOL CRoomManager::CreateRoom(CUser* host, std::string roomName)
{
	// ���� �� ��ŭ ������ ������.
	for (std::vector<CRoom*>::iterator iter = vRooms.begin(); iter != vRooms.end(); ++iter)
	{
		// ���� Ȱ��ȭ ���ִٸ�
		if (!(*iter)->GetRoomActive())
		{
			// �� ����� ó��.
			(*iter)->SetRoomActive(TRUE);
			(*iter)->SetRoomHost(host);
			(*iter)->SetRoomName(roomName);
			(*iter)->ConnectToRoom(host);
			
			activeRoomCount++;

			return TRUE;
		}
	}

	return FALSE;
}
