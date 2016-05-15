#pragma once

class CRoom;

//=================================================
// CChatServer
// Info : 유저 객체, 네트워크 처리를 위해 CPacketHandler를 상속받는다.
class CUser : public CPacketHandler
{
public:
	CUser();
	virtual ~CUser();

private :
	// 유저의 이름
	std::string		userName;
	// 유저의 상태
	eUserState		userState;
	// 접속한 방의 포인터
	CRoom*			connectedRoom;

	// 연결 상태
	BOOL				bConnected;
	// 유저의 하트비트
	INT					heartBeat;

public :
	// 초기화
	BOOL	Initialize(VOID);
	// 메모리 해제
	BOOL	Release(VOID);

	// User의 사용이 끝나서 접속을 받을 수 있는 상태로 되돌림.
	BOOL	Restore(SOCKET listenSocket);

	// 유저의 하트비트를 체크함.
	VOID	HeartBeatCheck(SOCKET listenSocket);

	// userName
	std::string	GetSectionUserName(VOID);
	VOID			SetSectionUserName(std::string userName);

	// userState
	eUserState	GetUserState(VOID);
	VOID			SetUserState(eUserState userState);

	// connectedRoom
	CRoom*		GetConnectedRoom(VOID);
	VOID			SetConnectedRoom(CRoom* room);

	// bConnected
	BOOL	GetConnected(VOID);
	VOID	SetConnected(BOOL connected);

	// heartBeat
	INT				GetHeartBeat(VOID);
	VOID			SetHeartBeat(INT count);
};

