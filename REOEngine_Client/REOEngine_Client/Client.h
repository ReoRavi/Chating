#pragma once

//===================================================
// CClient
// Info - 프로그램의 네트워크 처리를 담당하는 객체,
// Singleton을 상속받아 프로그램내에서 하나만 존재하는 인스턴스로 사용됨.
class CClient :	public CClientSection,
							public ISingleton<CClient>	
{
public:
	CClient();
	virtual ~CClient();

public :
	// 연결 유무
	BOOL		bConnected;

	// HeartBeat 쓰레드의 핸들
	HANDLE					heartbeatHandle;
	// HeartBeat 이벤트
	HANDLE					heartbeatEvent;

public :
	// 초기화
	BOOL		Initialize(VOID);
	// 메모리 해제
	BOOL		Release(VOID);

	// 하트비트를 체크할 쓰레드
	VOID	HeartBeatThread(VOID);

public:
	// CClientSection의 가상함수
	// 접속이 완료되었을 때 호출된다.
	VOID	OnIOConnect(VOID);
	// 접속이 끊겼을 때 호출된다.
	VOID	OnIODisconnect(VOID);

	// CClientSection의 가상함수, 
	// CClientSection에서 패킷을 받고 그 패킷을 처리할 동작들을 여기서 정의한다.
	VOID	PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer);

	// 로비 정보가 업데이트 됨.
	VOID	PT_PROC_LOBBY_INFOUPDATE(BYTE* buffer);
	// 방 연결 요청의 결과.
	VOID	PT_PROC_LOBBY_JOINROOM(BYTE* buffer);
	// 방 만들기 요청의 결과.
	VOID	PT_PROC_CREATEROOM_REQUEST(BYTE* buffer);
	// 방에서 메세지가 전달됨.
	VOID	PT_PROC_ROOM_MESSAGE(BYTE* buffer);		
	// 방에서 나감을 요청한 결과.
	VOID	PT_PROC_ROOM_EXIT(BYTE* buffer);
	// 하트비트 체크.
	VOID	PT_PROC_HEARTBEAT_CHECK(BYTE* buffer);
};

// 싱글톤으로 어디서든 접근 가능한 인스턴스 생성
#define	g_pNetwork	CClient::GetInstance()
