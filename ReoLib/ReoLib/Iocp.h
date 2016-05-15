#pragma once

//===============================================
// CIocp
// Info : IOCP를 정의함. 이 클래스를 상속받으면 IOCP를 사용하는 것임.
class CIocp
{
public:
	CIocp();
	virtual ~CIocp();

private :	
	// 쓰레드의 핸들
	std::vector<HANDLE>		threadHandle;
	// IOCP의 핸들
	HANDLE								hIOCP;

protected :
	// 초기화
	BOOL		Initialize(VOID);
	// 메모리 해제
	BOOL		Release(VOID);

	// 소켓을 IOCP에 등록한다. 소켓에 입출력이 발생하면 IOCP로부터 신호를 받을 수 있다.
	BOOL		RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);
	
public :
	// 실행될 IOCP의 쓰레드
	VOID		GameThreadCallBack(VOID);

protected :
	// IOCP의 입출력에 맞게 실행될 순수가상함수들.
	virtual	BOOL	IOConnected(VOID* object) = 0;
	virtual	BOOL	IODisconnected(VOID* object) = 0;
	virtual	VOID	IOReceived(VOID* object, DWORD receivedByte) = 0;
};

