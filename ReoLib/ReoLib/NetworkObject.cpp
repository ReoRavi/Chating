#include "headers.h"
#include "AutoCriticalSection.h"
#include "NetworkObject.h"

CNetworkObject::CNetworkObject()
{
	socket = NULL;
}

CNetworkObject::~CNetworkObject()
{
}

//==================================================
// Initialize
// Info : 초기화
BOOL CNetworkObject::Initialize(VOID)
{
	// 임계영역을 설정한다.
	CAutoCriticalSection section;

	if (socket)
	{
		CLog::WriteLog("CNetworkObject : Initialize - socket Error!\n");

		return FALSE;
	}

	ZeroMemory(&socketAddr, sizeof(SOCKADDR_IN));

	socket = NULL;

	// IOCP에 넘길 구조체를 구성한다.
	// 쓰레드에서 실행하는 루틴 실행 시, 이 구조체를 받을 수 있다.

	// 연결요청을 담당할 구조체
	accpetIOContext = new PER_IOCONTEXT;

	ZeroMemory(&accpetIOContext->overlapped, sizeof(OVERLAPPED));
	accpetIOContext->ioType = IO_TYPE::IO_ACCPET;
	accpetIOContext->object = this;

	// 수신을 담당할 구조체
	readIOContext = new PER_IOCONTEXT;

	ZeroMemory(&readIOContext->overlapped, sizeof(OVERLAPPED));
	readIOContext->ioType = IO_TYPE::IO_READ;
	readIOContext->object = this;

	// 송신을 담당할 구조체
	sendIOContext = new PER_IOCONTEXT;

	ZeroMemory(&sendIOContext->overlapped, sizeof(OVERLAPPED));
	sendIOContext->ioType = IO_TYPE::IO_WRITE;
	sendIOContext->object = this;

	ZeroMemory(readBuffer, sizeof(readBuffer));

	return TRUE;
}

//==================================================
// Release
// Info : 메모리 해제
BOOL CNetworkObject::Release(VOID)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	if (!socket)
		return FALSE;

	// 소켓을 멈춘다.
	shutdown(socket, SD_BOTH);

	closesocket(socket);

	socket = NULL;

	return TRUE;
}

//==================================================
// TCPSocket
// Info : 소켓을 TCP 옵션을 사용하도록 만든다.
BOOL CNetworkObject::TCPSocket(VOID)
{
	// 임계영역 설정.
	CAutoCriticalSection section;

	// IPv4, TCP 로 소켓을 생성한다.
	// 중첩 입출력이 가능하도록 WSA_FLAG_OVERLAPPED를 설정한다.
	socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (socket == INVALID_SOCKET)
	{
		CLog::WriteLog("CNetworkObject : TCPSocket - WSASocket() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Bind
// Info : 소켓에 주소를 할당한다.
// port - 소켓에 할당될 포트
BOOL CNetworkObject::Bind(USHORT port)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	//	Family - IPv4 형식을 사용한다.
	//	Port - 포트를 설정한다.
	//	s_addr - IP를 설정한다.
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 소켓에 주소, 프로토콜, 포트를 할당한다.
	if (bind(socket, (LPSOCKADDR)&socketAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Bind - bind() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Listen
// Info : 소켓이 연결요청을 받을 수 있는 상태로 만든다.
// backLog - 연결요청을 대기할 수 있는 소켓의 수.
BOOL CNetworkObject::Listen(INT backLog)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	//	소켓의 연결 대기열을 생성한다.
	if (listen(socket, backLog) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Listen - listen() Error!\n");

		return FALSE;
	}

	LINGER linger;
	// linger 옵션을 사용하도록 설정한다.
	linger.l_onoff = 1;
	// 우아한 종료를 실행하지 않는다. 종료하는 즉시 버퍼를 모두 버리고 종료한다.
	linger.l_linger = 0;

	// 소켓 옵션 설정, 비동기 Accpet와 linger 옵션을 세팅함.
	if (setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Listen - setsockopt() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Accept
// Info : 클라이언트의 연결 요청을 받는다.
// listenSocket - 연결 요청을 받을 listen 소켓.
BOOL CNetworkObject::Accept(SOCKET listenSocket)
{
	// 임계영역을 설정한다.
	CAutoCriticalSection section;

	// 클라이언트의 연결 요청을 받는다. AcceptEx로 변경하여 비동기식으로 전환.
	// 연결 요청을 풀링하여 받는다.
	if (AcceptEx(listenSocket, socket, readBuffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accpetIOContext->overlapped))
	{
		INT	ErrorCode = WSAGetLastError();

		// WSAEWOULDBLOCK - 즉시 완료되지 않았음. ERROR_IO_PENDING - 데이터가 전부 들어오지 않음.
		if (ErrorCode != ERROR_IO_PENDING && ErrorCode != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CNetworkObject : Accept - AcceptEx() Error!\n");

			return FALSE;
		}
	}

	return TRUE;
}

//==================================================
// Connect
// Info : 서버에 연결을 요청한다.
// address - 연결할 소켓의 주소.
// port - 연결할 소켓의 포트
BOOL CNetworkObject::Connect(const char* address, USHORT port)
{
	// 임계영역 처리
	CAutoCriticalSection section;

	if (!address || port <= 0)
	{
		CLog::WriteLog("CNetworkObject : Connect - address or port not apposite\n");

		return FALSE;
	}

	if (!socket)
	{
		CLog::WriteLog("CNetworkObject : Connect - socket not Allocationed\n");

		return FALSE;
	}

	//	Family - IPv4 형식을 사용한다.
	//	Port - 포트를 설정한다.
	//	s_addr - IP를 설정한다.
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = inet_addr(address);

	// 소켓에 연결요청을 한다. 
	if (WSAConnect(socket, (SOCKADDR*)&socketAddr, sizeof(socketAddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		// 즉시 완료되지 않았음.
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CNetworkObject : Connect - WSAConnect() Error\n ");

			return FALSE;
		}
	}

	return TRUE;
}

//==================================================
// Write
// Info : 데이터를 전송한다.
// data - 전송할 데이터
// datasize - 전송할 데이터의 크기
BOOL CNetworkObject::Write(BYTE* data, DWORD dataSize)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	DWORD	sendBytes = 0;
	DWORD	flags = 0;

	// WSASend 에 사용될 버퍼를 설정한다.
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)data;
	wsaBuf.len = dataSize;

	// 데이터 전송
	INT result = WSASend(socket, &wsaBuf, 1, &sendBytes, flags, 
											&sendIOContext->overlapped, NULL);

	if (result == SOCKET_ERROR || !WSAGetLastError() == WSA_IO_PENDING)
	{
		CLog::WriteLog("CNetworkObject : Write - WSASend() Error\n ");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// ReadFromIOCP
// Info : IOCP로 부터 데이터 버퍼를 읽어들인다.
// IOCP에서 자동적으로 데이터를 받고 (미리 요청 후, 데이터가 들어오면 반응함), readBuffer에 저장한 후에, 데이터를 받았다는 알림을 발생시킨다.
// data - 데이터 버퍼의 주소
// receivedByte - 데이터를 읽어들인 크기
BOOL CNetworkObject::ReadFromIOCP(BYTE* data, DWORD receivedByte)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	if (receivedByte <= 0)
		return FALSE;

	// readBuffer에 받은 데이터가 담겨있다. memcpy를 이용하여 데이터를 복사해준다.
	memcpy(data, readBuffer, receivedByte);

	return TRUE;
}

//==================================================
// ReadFromEventSelect
// Info : EventSelect로 부터 데이터 버퍼를 읽어들인다.
// IOCP와는 다르게 ReadFromEventSelect에서 직접 WSARecv를 호출해서 데이터를 받는다.
// data - 데이터 버퍼의 주소
// receivedLength - 읽어들인 데이터 버퍼의 크기
BOOL CNetworkObject::ReadFromEventSelect(BYTE* data, DWORD &receivedLength)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	DWORD	recvedBytes = 0;
	DWORD	flags = 0;

	// WSARecv에 사용될 버퍼를 설정한다.
	WSABUF	wsaBuf;
	wsaBuf.buf = (CHAR*)readBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	// 데이터를 받는다.
	if (WSARecv(socket, &wsaBuf, 1, &recvedBytes, &flags, &readIOContext->overlapped, NULL) == SOCKET_ERROR)
	{
		INT	ErrorCode = WSAGetLastError();

		// 중첩 연산은 가능하지만, 즉시 완료되지 않았다.
		if (ErrorCode != WSA_IO_PENDING && ErrorCode != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CNetworkObject : ReadFromEventSelect - WSARecv() Error\n ");

			return FALSE;
		}
	}

	// 받은 데이터 복사
	memcpy(data, readBuffer,recvedBytes);

	// 받은 데이터의 크기만큼 더한다. (비동기라서 한번에 데이터가 오지 않을수도 있음)
	receivedLength += recvedBytes;

	return TRUE;
}

//==================================================
// SetReadFromIOCP
// Info : IOCP가 데이터를 한번 읽고, 다시 데이터를 읽어들이기 위해 설정한다.
BOOL CNetworkObject::SetReadFromIOCP(VOID)
{
	// 임계영역 설정
	CAutoCriticalSection section;

	DWORD	recvedBytes = 0;
	DWORD	flags = 0;

	// WSARecv에 사용될 버퍼를 설정한다.
	// 클라이언트에서 데이터를 보내면, readBuffer에 데이터를 할당하고 수신 알림을 보낸다.
	WSABUF	wsaBuf;
	wsaBuf.buf = (CHAR*)readBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	// IOCP는 선 요청 후, 결과 처리를 한다.
	// WSARecv를 호출하여 메시지를 받겠다는 신호를 보낸다.
	if (WSARecv(socket, &wsaBuf, 1, &recvedBytes, &flags, &readIOContext->overlapped, NULL) == SOCKET_ERROR)
	{
		INT	ErrorCode = WSAGetLastError();

		// 중첩 연산은 가능하지만, 즉시 완료되지 않았다.
		if (ErrorCode != WSA_IO_PENDING && ErrorCode != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CNetworkObject : SetReadFromIOCP - WSARecv() Error\n ");

			return FALSE;
		}
	}

	return TRUE;
}

//==================================================
// GetSocket
// Info : 소켓을 받는다.
SOCKET CNetworkObject::GetSocket(VOID)
{
	return socket;
}

//==================================================
// SetSocket
// Info : 소켓을 설정한다.
VOID CNetworkObject::SetSocket(SOCKET socket)
{
	this->socket = socket;
}
