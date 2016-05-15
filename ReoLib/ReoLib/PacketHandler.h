#pragma once

//===============================================
// CPacketHandler
// Info : ��Ŷ �κ� ó���� ����ϴ� Ŭ����.
class CPacketHandler : public CNetworkObject
{
public:
	CPacketHandler();
	virtual ~CPacketHandler();

protected :
	// ��Ŷ�� ����
	BYTE			packetBuffer[MAX_BUFFER_LENGTH];
	// ���� ������ ũ��
	DWORD		receivedLength;

public :
	// �ʱ�ȭ
	BOOL	Initialize(VOID);
	// �޸� ����
	BOOL	Release(VOID);

	// ��Ŷ�� �����Ѵ�.
	BOOL	WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);
	
	// IOCP�� ���� ��Ŷ�� �о���δ�.
	BOOL	GetPacketFromIOCP(DWORD receivedByte);
	// EventSelect�� ���� ��Ŷ�� �о���̳�.
	BOOL	GetPacketFromEventSelect(VOID);

	// ���ο� ��Ŷ�� �ޱ� ���� �ʱ�ȭ�Ѵ�.
	BOOL	ResetPacket(VOID);

	// ���� ��Ŷ�� �м��Ѵ�.
	BOOL	AnalysisPacket(DWORD &packetLength, ePACKET_PROTOCOL &protocol, BYTE* buffer);
};

