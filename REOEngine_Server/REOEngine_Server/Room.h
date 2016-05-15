#pragma once

class CLobby;

//=================================================
// CRoom
// Info : 방, 로비안의 유저들을 관리한다.
class CRoom
{
public:
	CRoom();
	virtual ~CRoom();

private :
	// 방 안의 유저들을 관리할 List
	std::list<CUser*> users;

	// 방의 방장
	CUser*		host;
	// 방 안에 있는 유저들의 숫자
	INT				userCount;

	// 방의 번호
	INT				roomIndex;
	// 방의 이름
	std::string	roomName;

	// 방이 활성화됬는가.
	BOOL			bActive;

public :
	// 초기화
	BOOL		Initialize(INT index);
	// 메모리 해제
	BOOL		Release(VOID);

	// host
	CUser*	GetRoomHost(VOID);
	VOID		SetRoomHost(CUser* host);

	// userCount
	INT			GetRoomUserCount(VOID);
	VOID		SetRoomUserCount(INT count);

	// roomIndex
	INT			GetRoomIndex(VOID);
	VOID		SetRoomIndex(INT index);

	// roomName
	std::string	GetRoomName(VOID);
	VOID			SetRoomName(std::string roomName);

	// bActive
	BOOL		GetRoomActive(VOID);
	VOID		SetRoomActive(BOOL active);

	// 방에 접속한다.
	BOOL		ConnectToRoom(CUser* connectUser);
	// 방에서 나간다.
	BOOL		DisconnectToRoom(CUser* disconnectUser);

	// 방을 닫는다.
	BOOL		CloseRoom(CLobby* lobby);

	// 방을 리셋한다.
	BOOL		ResetRoom(VOID);

	// 방 안의 모든 유저들에게 패킷을 전송한다.
	BOOL		SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};

