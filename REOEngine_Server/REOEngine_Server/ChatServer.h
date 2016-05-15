#pragma once

//=================================================
// CChatServer
// Info : 메인 서버 객체, IOCP를 사용한다.
class CChatServer : public CIocp
{
public:
	CChatServer();
	virtual ~CChatServer();

public :
	// 서버 소켓 오브젝트.
	CNetworkObject*	listenObject;
	// 로비.
	CLobby*					lobby;
	// 유저 객체를 관리하는 매니저.
	CUserManager		userManager;

	// KeepAlive 쓰레드 핸들
	HANDLE					keepAliveHandle;
	// KeepAlive를 종료할 이벤트
	HANDLE					keepAliveDestroyEvent;

public :
	// 초기화
	BOOL		Initialize(VOID);
	// 메모리 해제
	BOOL		Release(VOID);

	// 메인 루프
	VOID		Loop(VOID);

	// 서버의 상태를 보여줌.
	VOID		ShowDataTable(VOID);

public :
	// KeepAlive 쓰레드
	VOID	KeepAliveThread(VOID);

	// IOCP 가상함수들 상속
	BOOL	IOConnected(VOID* object);
	BOOL	IODisconnected(VOID* object);
	VOID	IOReceived(VOID* object, DWORD receivedByte);

public :
	// 패킷 처리 함수들 구현

	// 로그인 요청.
	VOID PT_PROC_LOGIN_USERINFO(CUser* signaledObject, BYTE* buffer);
	// 방을 만든다.
	VOID PT_PROC_CREATEROOM_REQUEST(CUser* signaledObject, BYTE* buffer);
	// 로비 정보 업데이트.
	VOID PT_PROC_LOBBY_INFOUPDATE(CUser* signaledObject, BYTE* buffer);
	// 방에 접속함.
	VOID PT_PROC_LOBBY_JOINROOM(CUser* signaledObject, BYTE* buffer);
	// 방에 메세지가 전달됨
	VOID PT_PROC_ROOM_MESSAGE(CUser* signaledObject, BYTE* buffer);
	// 방에서 나감
	VOID PT_PROC_ROOM_EXIT(CUser* signaledObject, BYTE* buffer);
	// 클라이언트 하트비트 체크
	VOID PT_PROC_HEARTBEAT_CHECK(CUser* signaledObject, BYTE* buffer);
};

