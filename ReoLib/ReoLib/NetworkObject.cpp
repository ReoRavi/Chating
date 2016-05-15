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
// Info : �ʱ�ȭ
BOOL CNetworkObject::Initialize(VOID)
{
	// �Ӱ迵���� �����Ѵ�.
	CAutoCriticalSection section;

	if (socket)
	{
		CLog::WriteLog("CNetworkObject : Initialize - socket Error!\n");

		return FALSE;
	}

	ZeroMemory(&socketAddr, sizeof(SOCKADDR_IN));

	socket = NULL;

	// IOCP�� �ѱ� ����ü�� �����Ѵ�.
	// �����忡�� �����ϴ� ��ƾ ���� ��, �� ����ü�� ���� �� �ִ�.

	// �����û�� ����� ����ü
	accpetIOContext = new PER_IOCONTEXT;

	ZeroMemory(&accpetIOContext->overlapped, sizeof(OVERLAPPED));
	accpetIOContext->ioType = IO_TYPE::IO_ACCPET;
	accpetIOContext->object = this;

	// ������ ����� ����ü
	readIOContext = new PER_IOCONTEXT;

	ZeroMemory(&readIOContext->overlapped, sizeof(OVERLAPPED));
	readIOContext->ioType = IO_TYPE::IO_READ;
	readIOContext->object = this;

	// �۽��� ����� ����ü
	sendIOContext = new PER_IOCONTEXT;

	ZeroMemory(&sendIOContext->overlapped, sizeof(OVERLAPPED));
	sendIOContext->ioType = IO_TYPE::IO_WRITE;
	sendIOContext->object = this;

	ZeroMemory(readBuffer, sizeof(readBuffer));

	return TRUE;
}

//==================================================
// Release
// Info : �޸� ����
BOOL CNetworkObject::Release(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	if (!socket)
		return FALSE;

	// ������ �����.
	shutdown(socket, SD_BOTH);

	closesocket(socket);

	socket = NULL;

	return TRUE;
}

//==================================================
// TCPSocket
// Info : ������ TCP �ɼ��� ����ϵ��� �����.
BOOL CNetworkObject::TCPSocket(VOID)
{
	// �Ӱ迵�� ����.
	CAutoCriticalSection section;

	// IPv4, TCP �� ������ �����Ѵ�.
	// ��ø ������� �����ϵ��� WSA_FLAG_OVERLAPPED�� �����Ѵ�.
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
// Info : ���Ͽ� �ּҸ� �Ҵ��Ѵ�.
// port - ���Ͽ� �Ҵ�� ��Ʈ
BOOL CNetworkObject::Bind(USHORT port)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	//	Family - IPv4 ������ ����Ѵ�.
	//	Port - ��Ʈ�� �����Ѵ�.
	//	s_addr - IP�� �����Ѵ�.
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ���Ͽ� �ּ�, ��������, ��Ʈ�� �Ҵ��Ѵ�.
	if (bind(socket, (LPSOCKADDR)&socketAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Bind - bind() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Listen
// Info : ������ �����û�� ���� �� �ִ� ���·� �����.
// backLog - �����û�� ����� �� �ִ� ������ ��.
BOOL CNetworkObject::Listen(INT backLog)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	//	������ ���� ��⿭�� �����Ѵ�.
	if (listen(socket, backLog) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Listen - listen() Error!\n");

		return FALSE;
	}

	LINGER linger;
	// linger �ɼ��� ����ϵ��� �����Ѵ�.
	linger.l_onoff = 1;
	// ����� ���Ḧ �������� �ʴ´�. �����ϴ� ��� ���۸� ��� ������ �����Ѵ�.
	linger.l_linger = 0;

	// ���� �ɼ� ����, �񵿱� Accpet�� linger �ɼ��� ������.
	if (setsockopt(socket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		CLog::WriteLog("CNetworkObject : Listen - setsockopt() Error!\n");

		return FALSE;
	}

	return TRUE;
}

//==================================================
// Accept
// Info : Ŭ���̾�Ʈ�� ���� ��û�� �޴´�.
// listenSocket - ���� ��û�� ���� listen ����.
BOOL CNetworkObject::Accept(SOCKET listenSocket)
{
	// �Ӱ迵���� �����Ѵ�.
	CAutoCriticalSection section;

	// Ŭ���̾�Ʈ�� ���� ��û�� �޴´�. AcceptEx�� �����Ͽ� �񵿱������ ��ȯ.
	// ���� ��û�� Ǯ���Ͽ� �޴´�.
	if (AcceptEx(listenSocket, socket, readBuffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accpetIOContext->overlapped))
	{
		INT	ErrorCode = WSAGetLastError();

		// WSAEWOULDBLOCK - ��� �Ϸ���� �ʾ���. ERROR_IO_PENDING - �����Ͱ� ���� ������ ����.
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
// Info : ������ ������ ��û�Ѵ�.
// address - ������ ������ �ּ�.
// port - ������ ������ ��Ʈ
BOOL CNetworkObject::Connect(const char* address, USHORT port)
{
	// �Ӱ迵�� ó��
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

	//	Family - IPv4 ������ ����Ѵ�.
	//	Port - ��Ʈ�� �����Ѵ�.
	//	s_addr - IP�� �����Ѵ�.
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = inet_addr(address);

	// ���Ͽ� �����û�� �Ѵ�. 
	if (WSAConnect(socket, (SOCKADDR*)&socketAddr, sizeof(socketAddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		// ��� �Ϸ���� �ʾ���.
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
// Info : �����͸� �����Ѵ�.
// data - ������ ������
// datasize - ������ �������� ũ��
BOOL CNetworkObject::Write(BYTE* data, DWORD dataSize)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	DWORD	sendBytes = 0;
	DWORD	flags = 0;

	// WSASend �� ���� ���۸� �����Ѵ�.
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)data;
	wsaBuf.len = dataSize;

	// ������ ����
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
// Info : IOCP�� ���� ������ ���۸� �о���δ�.
// IOCP���� �ڵ������� �����͸� �ް� (�̸� ��û ��, �����Ͱ� ������ ������), readBuffer�� ������ �Ŀ�, �����͸� �޾Ҵٴ� �˸��� �߻���Ų��.
// data - ������ ������ �ּ�
// receivedByte - �����͸� �о���� ũ��
BOOL CNetworkObject::ReadFromIOCP(BYTE* data, DWORD receivedByte)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	if (receivedByte <= 0)
		return FALSE;

	// readBuffer�� ���� �����Ͱ� ����ִ�. memcpy�� �̿��Ͽ� �����͸� �������ش�.
	memcpy(data, readBuffer, receivedByte);

	return TRUE;
}

//==================================================
// ReadFromEventSelect
// Info : EventSelect�� ���� ������ ���۸� �о���δ�.
// IOCP�ʹ� �ٸ��� ReadFromEventSelect���� ���� WSARecv�� ȣ���ؼ� �����͸� �޴´�.
// data - ������ ������ �ּ�
// receivedLength - �о���� ������ ������ ũ��
BOOL CNetworkObject::ReadFromEventSelect(BYTE* data, DWORD &receivedLength)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	DWORD	recvedBytes = 0;
	DWORD	flags = 0;

	// WSARecv�� ���� ���۸� �����Ѵ�.
	WSABUF	wsaBuf;
	wsaBuf.buf = (CHAR*)readBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	// �����͸� �޴´�.
	if (WSARecv(socket, &wsaBuf, 1, &recvedBytes, &flags, &readIOContext->overlapped, NULL) == SOCKET_ERROR)
	{
		INT	ErrorCode = WSAGetLastError();

		// ��ø ������ ����������, ��� �Ϸ���� �ʾҴ�.
		if (ErrorCode != WSA_IO_PENDING && ErrorCode != WSAEWOULDBLOCK)
		{
			CLog::WriteLog("CNetworkObject : ReadFromEventSelect - WSARecv() Error\n ");

			return FALSE;
		}
	}

	// ���� ������ ����
	memcpy(data, readBuffer,recvedBytes);

	// ���� �������� ũ�⸸ŭ ���Ѵ�. (�񵿱�� �ѹ��� �����Ͱ� ���� �������� ����)
	receivedLength += recvedBytes;

	return TRUE;
}

//==================================================
// SetReadFromIOCP
// Info : IOCP�� �����͸� �ѹ� �а�, �ٽ� �����͸� �о���̱� ���� �����Ѵ�.
BOOL CNetworkObject::SetReadFromIOCP(VOID)
{
	// �Ӱ迵�� ����
	CAutoCriticalSection section;

	DWORD	recvedBytes = 0;
	DWORD	flags = 0;

	// WSARecv�� ���� ���۸� �����Ѵ�.
	// Ŭ���̾�Ʈ���� �����͸� ������, readBuffer�� �����͸� �Ҵ��ϰ� ���� �˸��� ������.
	WSABUF	wsaBuf;
	wsaBuf.buf = (CHAR*)readBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	// IOCP�� �� ��û ��, ��� ó���� �Ѵ�.
	// WSARecv�� ȣ���Ͽ� �޽����� �ްڴٴ� ��ȣ�� ������.
	if (WSARecv(socket, &wsaBuf, 1, &recvedBytes, &flags, &readIOContext->overlapped, NULL) == SOCKET_ERROR)
	{
		INT	ErrorCode = WSAGetLastError();

		// ��ø ������ ����������, ��� �Ϸ���� �ʾҴ�.
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
// Info : ������ �޴´�.
SOCKET CNetworkObject::GetSocket(VOID)
{
	return socket;
}

//==================================================
// SetSocket
// Info : ������ �����Ѵ�.
VOID CNetworkObject::SetSocket(SOCKET socket)
{
	this->socket = socket;
}
