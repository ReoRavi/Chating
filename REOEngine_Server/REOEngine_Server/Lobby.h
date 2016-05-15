#pragma once

//=================================================
// CLobby
// Info : 방, 로비안의 유저들을 관리한다.
class CLobby
{
public:
	CLobby();
	virtual ~CLobby();

private :
	// 방을 관리하는 매니저 객체.
	CRoomManager*	roomManager;
	// 로비들의 유저들을 관리하는 List.
	std::list<CUser*>	users;
	// 로비안의 유저들의 카운트.
	INT							userCount;

public:
	// 초기화.
	BOOL	Initialize(VOID);
	// 메모리 해제.
	BOOL	Release(VOID);

	// roomManager
	CRoomManager	*	GetRoomManager(VOID);
	VOID						SetRoomManager(CRoomManager* roomManager);

	// userCount 
	INT			GetUserCount(VOID);
	VOID		SetUserCount(INT count);

	// 유저를 로비에 연결한다.
	BOOL		ConnectToLobby(CUser* connectedUser);
	// 유저를 로비에서 연결 해제한다.
	BOOL		DisconnectToLobby(CUser* disconnectedUser);

	// 방을 만든다.
	BOOL		CreateRoom(CUser* host, std::string roomName);

	// 로비안의 모든 유저에게 패킷을 보낸다.
	BOOL		SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};

