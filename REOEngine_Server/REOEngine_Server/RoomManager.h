#pragma once

//=================================================
// CRoomManager
// Info : 방을 관리하는 매니저 객체
class CRoomManager
{
public:
	CRoomManager();
	virtual ~CRoomManager();

private :
	// 방을 관리하는 Vector
	std::vector<CRoom*>	vRooms;
	// 활성화된 방의 수
	INT activeRoomCount;
	
public:
	// 초기화
	BOOL	Initialize();
	// 메모리 해제
	BOOL	Release();

	// vRoom
	CRoom*	GetRoom(INT index);
	VOID		SetRoom(CRoom* room, INT index);

	// activeRoomCount
	INT			GetActiveRoomCount();
	VOID		SetActiveRoomCount(INT count);

public :
	// 방을 만든다.
	BOOL	CreateRoom(CUser* host, std::string roomName);
};

