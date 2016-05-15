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
// Info : 초기화
BOOL CRoomManager::Initialize()
{
	// 미리 방을 생성해둔다.
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
// Info : 메모리 해제
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
// Info : 방을 리턴한다.
// index - 얻을 방의 인덱스
CRoom* CRoomManager::GetRoom(INT index)
{
	return vRooms[index];
}

//=================================================
// SetRoom
// Info : 방을 설정한다
// room - 설정할 방의 포인터
// index - 설정할 방의 인덱스
VOID CRoomManager::SetRoom(CRoom* room, INT index)
{
	vRooms[index] = room;
}

//=================================================
// GetActiveRoomCount
// Info : 활성화된 방의 카운트를 리턴한다.
INT CRoomManager::GetActiveRoomCount()
{
	return activeRoomCount;
}

//=================================================
// SetActiveRoomCount
// Info : 활성화된 방의 카운트를 설정한다.
// count - 설정할 카운트 값
VOID CRoomManager::SetActiveRoomCount(INT count)
{
	activeRoomCount = count;
}

//=================================================
// CreateRoom
// Info : 방을 만든다.
// host - 방장의 포인터.
// roomName - 방의 이름
BOOL CRoomManager::CreateRoom(CUser* host, std::string roomName)
{
	// 방의 수 만큼 루프를 돌린다.
	for (std::vector<CRoom*>::iterator iter = vRooms.begin(); iter != vRooms.end(); ++iter)
	{
		// 방이 활성화 되있다면
		if (!(*iter)->GetRoomActive())
		{
			// 방 만들기 처리.
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
