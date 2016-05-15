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
// Info : TCP������ ����ϰ�, ��Ʈ��ũ ����� ���� �ʱ�ȭ�� �����Ѵ�.
BOOL	CClientSection::InitializeTCP(VOID)
{
	// �޸� �Ҵ�
	serverSection = new CPacketHandler();

	// ���� ��ü �ʱ�ȭ.
	if (!serverSection->Initialize())
	{
		CLog::WriteLog("CClientSection : InitializeTCP - serverSection->Initialize() Error!\n");

		return FALSE;
	}

	// TCP ���� ����
	if (!serverSection->TCPSocket())
	{
		CLog::WriteLog("CClientSection : InitializeTCP - serverSection->TCPSocket() Error!\n");

		return FALSE;
	}

	// EventSelect�� ���� ���� �ּҸ� �Ѱ��ְ�, �ʱ�ȭ.
	if (!CEventSelect::Initialize(serverSection->GetSocket()))
	{
		CLog::WriteLog("CClientSection : InitializeTCP - CEventSelect::Initialize() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Release
// Info : �޸� ����.
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
// Info : ������ ���� ��û�� �Ѵ�.
// address - �����û�� ������ �ּ�
// port - �����û�� ������ ��Ʈ
BOOL CClientSection::ConnectToServer(const char* address, USHORT port)
{
	// ������ ���� ��û�� �Ѵ�.
	if (!serverSection->Connect(address, port))
	{
		CLog::WriteLog("CClientSection : serverSection->Connect() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// WritePacket
// Info : ��Ŷ�� ������.
// length - ��Ŷ�� ����
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
BOOL CClientSection::WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// ������ ��Ŷ�� ������. 
	if (!serverSection->WritePacket(length, protocol, buffer))
	{
		CLog::WriteLog("CClientSection : serverSection->WritePacket() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// OnIORead
// Info : EventSelect�� FD_READ �̺�Ʈ�� �۵��ϴ� �Լ�.
VOID CClientSection::OnIORead(VOID)
{
	BYTE buffer[MAX_BUFFER_LENGTH] = { 0, };

	// EventSelect���κ��� ��Ŷ�� �о�´�. ��ü ��Ŷ�� �� �о��� ��쿡�� TRUE�� ����.
	if (serverSection->GetPacketFromEventSelect())
	{
		DWORD								packetLength = 0;
		ePACKET_PROTOCOL	protocol;
		BYTE									buffer[MAX_BUFFER_LENGTH] = { 0, };

		// �޾ƿ� ��Ŷ �м�.
		if (serverSection->AnalysisPacket(packetLength, protocol, buffer))
		{
			PacketProc(protocol, buffer);
		}

		// ��Ŷ�� ���� �޾�������, �ʱ�ȭ�Ѵ�.
		serverSection->ResetPacket();
	}
}

//==================================================
// OnIOWrite
// Info : EventSelect�� FD_WRITE �̺�Ʈ�� �۵��ϴ� �Լ�.
VOID CClientSection::OnIOWrite(VOID)
{
	// ������ ����.
}