#pragma once

//===============================================
// CNetworkObject
// Info : ��Ʈ��ũ �κ� ó���� ����ϴ� ������Ʈ.
class CNetworkObject
{
public:
	CNetworkObject();
	virtual ~CNetworkObject();

private :
	// ��Ʈ��ũ ó���� ���� ����
	SOCKET				socket;
	// ������ �ּ�
	SOCKADDR_IN	socketAddr;

	// IOCP�� ���� ��û�� ���� ����ü.
	PPER_IOCONTEXT		accpetIOContext;
	// IOCP�� ���ſ� ���� ����ü.
	PPER_IOCONTEXT		readIOContext;
	// IOCP�� �۽ſ� ���� ����ü.
	PPER_IOCONTEXT		sendIOContext;

protected :
	// �������κ��� �о���� ������ ����.
	BYTE			readBuffer[MAX_BUFFER_LENGTH];

public :
	// �ʱ�ȭ
	BOOL		Initialize(VOID);
	// �޸� ����
	BOOL		Release(VOID);

	// TCP�� ����ϴ� ������ �����Ѵ�.
	BOOL		TCPSocket(VOID);

	// ���Ͽ� �ּҸ� �Ҵ��Ѵ�.
	BOOL		Bind(USHORT port);
	// ������ �����û�� ���� �� �ֵ��� �����·� �����.
	BOOL		Listen(INT backLog);

	// Ŭ���̾�Ʈ�� ���� ��û�� �޴´�.
	BOOL		Accept(SOCKET listenSocket);

	// ������ ������ ��û�Ѵ�.
	BOOL		Connect(const char* address, USHORT port);

protected :
	// ���Ͽ� �����͸� �����Ѵ�.
	BOOL		Write(BYTE* data, DWORD dataSize);

public :
	// IOCP�� ���� ������ ���۸� �о���δ�.
	BOOL		ReadFromIOCP(BYTE* data, DWORD receivedByte);
	// EventSelect�� ���� ������ ���۸� �о���δ�.
	BOOL		ReadFromEventSelect(BYTE* data, DWORD &receivedLength);

	// IOCP�� �ٽ� �����͸� �о���� �� �ֵ��� �����Ѵ�.
	BOOL		SetReadFromIOCP(VOID);

public :
	// ������ �����Ѵ�.
	SOCKET	GetSocket(VOID);
	// ������ �����Ѵ�.
	VOID			SetSocket(SOCKET socket);
};

