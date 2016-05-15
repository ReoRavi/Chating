#include "Headers.h"
#include "NetworkObject.h"
#include "PacketHandler.h"
#include "EventSelect.h"
#include "ClientSection.h"

CClientSection::CClientSection()
{
}


CClientSection::~CClientSection()
{
}

//==================================================
// InitializeTCP
// Info : TCP소켓을 사용하고, 네트워크 통신을 위한 초기화를 진행한다.
BOOL	CClientSection::InitializeTCP(VOID)
{
	// 메모리 할당
	serverSection = new CPacketHandler();

	// 서버 객체 초기화.
	if (!serverSection->Initialize())
	{
		CLog::WriteLog("CClientSection : InitializeTCP - serverSection->Initialize() Error!\n");

		return FALSE;
	}

	// TCP 소켓 생성
	if (!serverSection->TCPSocket())
	{
		CLog::WriteLog("CClientSection : InitializeTCP - serverSection->TCPSocket() Error!\n");

		return FALSE;
	}

	// EventSelect에 사용될 소켓 주소를 넘겨주고, 초기화.
	if (!CEventSelect::Initialize(serverSection->GetSocket()))
	{
		CLog::WriteLog("CClientSection : InitializeTCP - CEventSelect::Initialize() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Release
// Info : 메모리 해제.
BOOL	CClientSection::Release(VOID)
{
	if (serverSection->Release())
	{
		return FALSE;
	}

	if (CEventSelect::Release())
	{
		return FALSE;
	}

	delete serverSection;

	return TRUE;
}

//==================================================
// ConnectToServer
// Info : 서버에 접속 요청을 한다.
// address - 연결요청할 서버의 주소
// port - 연결요청할 서버의 포트
BOOL CClientSection::ConnectToServer(const char* address, USHORT port)
{
	// 서버에 접속 요청을 한다.
	if (!serverSection->Connect(address, port))
	{
		CLog::WriteLog("CClientSection : serverSection->Connect() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// WritePacket
// Info : 패킷을 보낸다.
// length - 패킷의 길이
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
BOOL CClientSection::WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 서버에 패킷을 보낸다. 
	if (!serverSection->WritePacket(length, protocol, buffer))
	{
		CLog::WriteLog("CClientSection : serverSection->WritePacket() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// OnIORead
// Info : EventSelect의 FD_READ 이벤트에 작동하는 함수.
VOID CClientSection::OnIORead(VOID)
{
	BYTE buffer[MAX_BUFFER_LENGTH] = { 0, };

	// EventSelect으로부터 패킷을 읽어온다. 전체 패킷을 다 읽었을 경우에만 TRUE가 리턴.
	if (serverSection->GetPacketFromEventSelect())
	{
		DWORD								packetLength = 0;
		ePACKET_PROTOCOL	protocol;
		BYTE									buffer[MAX_BUFFER_LENGTH] = { 0, };

		// 받아온 패킷 분석.
		if (serverSection->AnalysisPacket(packetLength, protocol, buffer))
		{
			PacketProc(protocol, buffer);
		}

		// 패킷을 전부 받았음으로, 초기화한다.
		serverSection->ResetPacket();
	}
}

//==================================================
// OnIOWrite
// Info : EventSelect의 FD_WRITE 이벤트에 작동하는 함수.
VOID CClientSection::OnIOWrite(VOID)
{
	// 사용되지 않음.
}