#pragma once

//===============================================
// CPacketHandler
// Info : 패킷 부분 처리를 담당하는 클래스.
class CPacketHandler : public CNetworkObject
{
public:
	CPacketHandler();
	virtual ~CPacketHandler();

protected :
	// 패킷의 버퍼
	BYTE			packetBuffer[MAX_BUFFER_LENGTH];
	// 받은 버퍼의 크기
	DWORD		receivedLength;

public :
	// 초기화
	BOOL	Initialize(VOID);
	// 메모리 해제
	BOOL	Release(VOID);

	// 패킷을 전송한다.
	BOOL	WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
	
	// IOCP로 부터 패킷을 읽어들인다.
	BOOL	GetPacketFromIOCP(DWORD receivedByte);
	// EventSelect로 부터 패킷을 읽어들이낟.
	BOOL	GetPacketFromEventSelect(VOID);

	// 새로운 패킷을 받기 위해 초기화한다.
	BOOL	ResetPacket(VOID);

	// 받은 패킷을 분석한다.
	BOOL	AnalysisPacket(DWORD &packetLength, ePACKET_PROTOCOL &protocol, BYTE* buffer);
};

