#pragma once

//===============================================
// CNetworkObject
// Info : 네트워크 부분 처리를 담당하는 오브젝트.
class CNetworkObject
{
public:
	CNetworkObject();
	virtual ~CNetworkObject();

private :
	// 네트워크 처리에 사용될 소켓
	SOCKET				socket;
	// 소켓의 주소
	SOCKADDR_IN	socketAddr;

	// IOCP의 연결 요청에 사용될 구조체.
	PPER_IOCONTEXT		accpetIOContext;
	// IOCP의 수신에 사용될 구조체.
	PPER_IOCONTEXT		readIOContext;
	// IOCP의 송신에 사용될 구조체.
	PPER_IOCONTEXT		sendIOContext;

protected :
	// 수신으로부터 읽어들인 데이터 버퍼.
	BYTE			readBuffer[MAX_BUFFER_LENGTH];

public :
	// 초기화
	BOOL		Initialize(VOID);
	// 메모리 해제
	BOOL		Release(VOID);

	// TCP를 사용하는 소켓을 생성한다.
	BOOL		TCPSocket(VOID);

	// 소켓에 주소를 할당한다.
	BOOL		Bind(USHORT port);
	// 소켓이 연결요청을 받을 수 있도록 대기상태로 만든다.
	BOOL		Listen(INT backLog);

	// 클라이언트의 연결 요청을 받는다.
	BOOL		Accept(SOCKET listenSocket);

	// 서버에 연결을 요청한다.
	BOOL		Connect(const char* address, USHORT port);

protected :
	// 소켓에 데이터를 전송한다.
	BOOL		Write(BYTE* data, DWORD dataSize);

public :
	// IOCP로 부터 데이터 버퍼를 읽어들인다.
	BOOL		ReadFromIOCP(BYTE* data, DWORD receivedByte);
	// EventSelect로 부터 데이터 버퍼를 읽어들인다.
	BOOL		ReadFromEventSelect(BYTE* data, DWORD &receivedLength);

	// IOCP가 다시 데이터를 읽어들일 수 있도록 설정한다.
	BOOL		SetReadFromIOCP(VOID);

public :
	// 소켓을 리턴한다.
	SOCKET	GetSocket(VOID);
	// 소켓을 설정한다.
	VOID			SetSocket(SOCKET socket);
};

