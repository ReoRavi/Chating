#pragma once

//=================================================
// CUserManager
// Info : 유저들의 객체를 관리하는 매니저
class CUserManager
{
public:
	CUserManager();
	virtual ~CUserManager();

private :
	// 유저 객체들을 관리하는 Vector
	std::vector<CUser*> vUsers;

public :
	// 접속해있는 유저 숫자
	INT userCount;

public :
	// 초기화
	BOOL Initialize(SOCKET listenSocket);
	// 메모리 해제
	BOOL	Release(VOID);

	// 모든 유저들의 하트비트를 체크한다.
	VOID	CheckAllHeartBeat(SOCKET listenSocket);
	// 모든 유저들에게 패킷을 보낸다.
	BOOL	SendToAll(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
};

