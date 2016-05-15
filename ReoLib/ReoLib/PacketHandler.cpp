#include "headers.h"
#include "NetworkObject.h"
#include "AutoCriticalSection.h"
#include "PacketHandler.h"
#include "XOREncrypt.h"

CPacketHandler::CPacketHandler()
{
	ZeroMemory(packetBuffer, sizeof(packetBuffer));
	receivedLength = 0;
}


CPacketHandler::~CPacketHandler()
{
}

//==================================================
// Initialize
// Info : �ʱ�ȭ
BOOL CPacketHandler::Initialize(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	ZeroMemory(packetBuffer, sizeof(packetBuffer));

	receivedLength = 0;

	if (!CNetworkObject::Initialize())
	{
		CLog::WriteLog("CPacketHandler : Initialize - CNetworkObject::Initialize() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Release
// Info : �޸� ����
BOOL CPacketHandler::Release(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	if (!CNetworkObject::Release())
	{
		CLog::WriteLog("CPacketHandler : Release - CNetworkObject::Release() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// WritePacket
// Info : ��Ŷ�� �����, ������.
// length - ��Ŷ�� ����
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
BOOL CPacketHandler::WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// ���� �Է�
	memcpy(packetBuffer, &length, sizeof(DWORD));

	// �������� �Է�
	memcpy(packetBuffer + sizeof(DWORD), &protocol, sizeof(ePACKET_PROTOCOL));

	// ��ȣȭ, ���� �κи� ��ȣȭ�Ѵ�.
	if (!CXOREncrypt::Encrypt(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), length))
		return FALSE;

	// ���� �Է�
	memcpy(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), buffer, length);

	// ���Ͽ� ��Ŷ�� �����Ѵ�. ������ �Ϸ�Ǹ� ��Ŷ�� �����Ѵ�.
	return CNetworkObject::Write(packetBuffer, length + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL)) && ResetPacket();
}

//==================================================
// GetPacketFromIOCP
// Info : ��Ŷ�� IOCP�κ��� �о���δ�.
// receivedByte - �о���� ������ ũ��
BOOL CPacketHandler::GetPacketFromIOCP(DWORD receivedByte)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// �о���� ������ ���� ��ŭ ��ģ��.(�񵿱� IO �̱� ������ �ѹ��� �������� �� ����)
	if (CNetworkObject::ReadFromIOCP(packetBuffer + receivedLength, receivedByte))
	{
		// ���� �ޱ�
		DWORD	packetLength;

		receivedLength += receivedByte;

		// �ּ����� ���۸� ���� ������ ���.
		if (receivedLength < sizeof(DWORD))
			return FALSE;

		memcpy(&packetLength, readBuffer, sizeof(DWORD));

		// ��Ŷ�� ���� ����. ���۵Ǵ°��� ������ ��Ŷ�� ������, ���̿� ���������� ũ��� ������.
		if (receivedLength - sizeof(DWORD) - sizeof(ePACKET_PROTOCOL) >= packetLength)
			return TRUE;
	}

	return FALSE;
}

//==================================================
// GetPacketFromEventSelect
// Info : EventSelect�� ���� ��Ŷ�� �о���δ�.
BOOL CPacketHandler::GetPacketFromEventSelect(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// �о���� ������ ���� ��ŭ ��ģ��.(�񵿱� IO �̱� ������ �ѹ��� �������� �� ����)
	if (CNetworkObject::ReadFromEventSelect(packetBuffer + receivedLength, receivedLength))
	{
		DWORD packetLength;

		// �ּ����� �ؼ������� ��Ŷ�� ���Դ��� Ȯ��.
		if (receivedLength <= sizeof(DWORD))
			return FALSE;

		// ��Ŷ�� ���̸� �޴´�.
		memcpy(&packetLength, packetBuffer, sizeof(DWORD));

		// ��Ŷ ���̸� �ް�, ��Ŷ�� �� �޾Ҵ��� Ȯ��. �� �޾Ҵٸ� TRUE ����
		if (receivedLength >= packetLength)
			return TRUE;
	}

	return FALSE;
}

//==================================================
// ResetPacket
// Info : ���ο� ��Ŷ�� �ޱ� ���� ��Ŷ�� �ʱ�ȭ�Ѵ�.
BOOL CPacketHandler::ResetPacket(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	memset(packetBuffer, 0, sizeof(packetBuffer));
	receivedLength = 0;

	return TRUE;
}

//==================================================
// AnalysisPacket
// Info : ��Ŷ�� �м��Ѵ�.
// packetLength - ��Ŷ�� ����
// protocol - ��Ŷ�� ��������
// buffer - ��Ŷ�� ����
BOOL	CPacketHandler::AnalysisPacket(DWORD &packetLength, ePACKET_PROTOCOL &protocol, BYTE* buffer)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	// ���� �ޱ�
	memcpy(&packetLength, packetBuffer, sizeof(DWORD));

	// �������� �ޱ�
	memcpy(&protocol, packetBuffer + sizeof(DWORD), sizeof(DWORD));

	// ���� �ޱ�
	memcpy(buffer, packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), packetLength);

	// ���� ���۸� ��ȣȭ�Ѵ�.
	if (!CXOREncrypt::Decrypt(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), packetLength))
		return FALSE;

	return TRUE;
}


