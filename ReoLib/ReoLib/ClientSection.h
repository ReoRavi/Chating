#pragma once

//===============================================
// CClientSection
// Info : EventSelect�� ��ӹ޴� Ŭ����. Ŭ���̾�Ʈ�� ��Ʈ��ũ ó���� ����Ѵ�.
class CClientSection : public CEventSelect
{
public:
	CClientSection();
	virtual ~CClientSection();

protected:
	// ���� ���ϰ� �����κ� ó���� ������ִ� ����.
	CPacketHandler* serverSection;

public :
	// TCP������ ����ϰ�, Ŭ������ �ʱ�ȭ ��.
	BOOL	InitializeTCP(VOID);
	// �޸� ����.
	BOOL	Release(VOID);

	// ��Ŷ�� ������.
	BOOL	WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer);

	// ������ ����.
	BOOL	ConnectToServer(const char* address, USHORT port);


protected:
	// CEventSelect�� ���������Լ�. 
	// EventSelect���� ����� �˸��� ���� �� �Լ����� ������.
	VOID	OnIORead(VOID);
	VOID	OnIOWrite(VOID);

protected :
	// Ŭ���̾�Ʈ���� ����� ��Ŷ ó�� �Լ�.
	// ó���ؾ��� ��Ŷ ������ ������Ʈ���� �ٸ����� ���������Լ��� ���� ����.
	virtual VOID PacketProc(ePACKET_PROTOCOL protocol, BYTE* buffer) = 0;
};

