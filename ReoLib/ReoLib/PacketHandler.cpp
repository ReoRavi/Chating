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
// Info : 초기화
BOOL CPacketHandler::Initialize(VOID)
{
	// 임계영역 설정
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
// Info : 메모리 해제
BOOL CPacketHandler::Release(VOID)
{
	// 임계영역 설정
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
// Info : 패킷을 만들고, 보낸다.
// length - 패킷의 길이
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
BOOL CPacketHandler::WritePacket(DWORD length, ePACKET_PROTOCOL protocol, BYTE* buffer)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 길이 입력
	memcpy(packetBuffer, &length, sizeof(DWORD));

	// 프로토콜 입력
	memcpy(packetBuffer + sizeof(DWORD), &protocol, sizeof(ePACKET_PROTOCOL));

	// 암호화, 버퍼 부분만 암호화한다.
	if (!CXOREncrypt::Encrypt(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), length))
		return FALSE;

	// 버퍼 입력
	memcpy(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), buffer, length);

	// 소켓에 패킷을 전송한다. 전송이 완료되면 패킷을 리셋한다.
	return CNetworkObject::Write(packetBuffer, length + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL)) && ResetPacket();
}

//==================================================
// GetPacketFromIOCP
// Info : 패킷을 IOCP로부터 읽어들인다.
// receivedByte - 읽어들인 데이터 크기
BOOL CPacketHandler::GetPacketFromIOCP(DWORD receivedByte)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 읽어들인 버퍼의 길이 만큼 합친다.(비동기 IO 이기 때문에 한번에 오지않을 수 있음)
	if (CNetworkObject::ReadFromIOCP(packetBuffer + receivedLength, receivedByte))
	{
		// 길이 받기
		DWORD	packetLength;

		receivedLength += receivedByte;

		// 최소한의 버퍼를 받지 못했을 경우.
		if (receivedLength < sizeof(DWORD))
			return FALSE;

		memcpy(&packetLength, readBuffer, sizeof(DWORD));

		// 패킷을 전부 받음. 전송되는것은 데이터 패킷의 사이즈, 길이와 프로토콜의 크기는 빼야함.
		if (receivedLength - sizeof(DWORD) - sizeof(ePACKET_PROTOCOL) >= packetLength)
			return TRUE;
	}

	return FALSE;
}

//==================================================
// GetPacketFromEventSelect
// Info : EventSelect로 부터 패킷을 읽어들인다.
BOOL CPacketHandler::GetPacketFromEventSelect(VOID)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 읽어들인 버퍼의 길이 만큼 합친다.(비동기 IO 이기 때문에 한번에 오지않을 수 있음)
	if (CNetworkObject::ReadFromEventSelect(packetBuffer + receivedLength, receivedLength))
	{
		DWORD packetLength;

		// 최소한의 해석가능한 패킷이 들어왔는지 확인.
		if (receivedLength <= sizeof(DWORD))
			return FALSE;

		// 패킷의 길이를 받는다.
		memcpy(&packetLength, packetBuffer, sizeof(DWORD));

		// 패킷 길이를 받고, 패킷을 다 받았는지 확인. 다 받았다면 TRUE 리턴
		if (receivedLength >= packetLength)
			return TRUE;
	}

	return FALSE;
}

//==================================================
// ResetPacket
// Info : 새로운 패킷을 받기 위해 패킷을 초기화한다.
BOOL CPacketHandler::ResetPacket(VOID)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	memset(packetBuffer, 0, sizeof(packetBuffer));
	receivedLength = 0;

	return TRUE;
}

//==================================================
// AnalysisPacket
// Info : 패킷을 분석한다.
// packetLength - 패킷의 길이
// protocol - 패킷의 프로토콜
// buffer - 패킷의 버퍼
BOOL	CPacketHandler::AnalysisPacket(DWORD &packetLength, ePACKET_PROTOCOL &protocol, BYTE* buffer)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	// 길이 받기
	memcpy(&packetLength, packetBuffer, sizeof(DWORD));

	// 프로토콜 받기
	memcpy(&protocol, packetBuffer + sizeof(DWORD), sizeof(DWORD));

	// 버퍼 받기
	memcpy(buffer, packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), packetLength);

	// 받은 버퍼를 복호화한다.
	if (!CXOREncrypt::Decrypt(packetBuffer + sizeof(DWORD) + sizeof(ePACKET_PROTOCOL), packetLength))
		return FALSE;

	return TRUE;
}


