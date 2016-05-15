#pragma once

//===============================================
// CClientSection
// Info : EventSelect를 상속받는 클래스. 클라이언트의 네트워크 처리를 담당한다.
class CClientSection : public CEventSelect
{
public:
	CClientSection();
	virtual ~CClientSection();

protected:
	// 실제 소켓과 서버부분 처리를 담당해주는 변수.
	CPacketHandler* serverSection;

public :
	// TCP소켓을 사용하고, 클래스를 초기화 함.
	BOOL	InitializeTCP(VOID);
	// 메모리 해제.
	BOOL	Release(VOID);

	// 패킷을 보낸다.
	BOOL	WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);

	// 서버에 접속.
	BOOL	ConnectToServer(const char* address, USHORT port);


protected:
	// CEventSelect의 순수가상함수. 
	// EventSelect에서 입출력 알림이 오면 이 함수들을 실행함.
	VOID	OnIORead(VOID);
	VOID	OnIOWrite(VOID);

protected :
	// 클라이언트에서 사용할 패킷 처리 함수.
	// 처리해야할 패킷 내용이 프로젝트마다 다름으로 순수가상함수를 통해 구현.
	virtual VOID PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer) = 0;
};

